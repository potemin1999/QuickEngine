/*
 *  Only for personal usage
 */

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Camera.h>


#include "util/Log.h"

using namespace QE;


class QE::CameraData {
public:

    CameraData() = default;

    ~CameraData() = default;

    bool trackVerticalAxis = true;

    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;

    float lastPitch = 0.0f;
    float lastYaw = 0.0f;
    float lastRoll = 0.0f;


    float aspect = 1.0f;
    float fovy = 2.0f;
    float farZ = 40.0f;
    float nearZ = 0.1f;

    /**
     * camera orientation quaternion
     */
    glm::quat qOrientation;

    /**
     * camera position
     */
    glm::vec3 mPos;


    glm::vec3 mStartView;
    /**
     * normalized vector of view
     */
    glm::vec3 mView;

    /**
     * up vector
     */
    glm::vec3 mUp;

    /**
     * matrix of view
     */
    glm::mat4 mViewMatrix;

    /**
     * matrix to project vertices
     */
    glm::mat4 mProjectionMatrix;

    /**
     * matrix = [ model × view ] × projection
     */
    glm::mat4 mVPMatrix;


};

namespace QE {

    namespace CameraImpl {

        void computeViewVector(CameraData *data) {
            if (data->trackVerticalAxis) {
                auto qPitch = glm::angleAxis(data->pitch, glm::vec3(-1, 0, 0));
                auto qYaw = glm::angleAxis(data->yaw, glm::vec3(0, 1, 0));
                data->qOrientation = qPitch * qYaw;
                auto rollAxis = glm::conjugate(data->qOrientation) * glm::vec3(0, 0, 1);
                auto qRoll = glm::angleAxis(data->roll, rollAxis);
                data->qOrientation *= qRoll;
                data->qOrientation = glm::normalize(data->qOrientation);
                data->mView = glm::rotate(data->qOrientation, data->mStartView);
            } else {
                auto vDelta = glm::vec3(data->lastPitch - data->pitch,
                                        data->yaw - data->lastYaw,
                                        data->roll - data->lastRoll);
                auto qDelta = glm::quat(vDelta);
                data->lastPitch = data->pitch;
                data->lastYaw = data->yaw;
                data->lastRoll = data->roll;
                data->qOrientation = qDelta * data->qOrientation;
                data->qOrientation = glm::normalize(data->qOrientation);
                data->mView = glm::rotate(qDelta, data->mView);
            }
        }

        /*
        * computes [view_matrix] by eye position, up vector and mRotation angles
        */
        void computeLook(Camera *camera) {
            CameraData *data = camera->data;
            computeViewVector(data);
            auto mRotate = glm::mat4_cast(data->qOrientation);
            auto mTranslate = glm::mat4(1.0f);
            mTranslate = glm::translate(mTranslate, -data->mPos);
            data->mViewMatrix = mRotate * mTranslate;

            //log("p: %f, y: %f, r: %f\n", camera->pitch, camera->yaw, camera->roll);
        }

    }

}


Camera::Camera() {
    data = new CameraData();
}

Camera::~Camera() {
    delete data;
}

float Camera::invSqrt(float x) {
    float half = x * 0.5f;
    int i = *(int *) &x;
    i = 0x5f3759df - (i >> 1);
    x = *(float *) &i;
    x = x * (1.5f - half * x * x);
    return x;
}

void Camera::setTrackVerticalAxis(bool track) {
    data->trackVerticalAxis = track;
}

void Camera::setPerspective(float aspect, float fovy, float farZ) {
    data->aspect = aspect;
    data->fovy = fovy;
    data->farZ = farZ;
    data->nearZ = 0.01f;
    data->mProjectionMatrix = glm::perspective(data->fovy, data->aspect, data->nearZ, data->farZ);
}

void Camera::setCameraRotation(float pitch, float yaw, float roll) {
    data->pitch = pitch;
    data->yaw = yaw;
    data->roll = roll;
    data->lastPitch = pitch;
    data->lastRoll = roll;
    data->lastYaw = yaw;
}

void Camera::addCameraRotation(float dPitch, float dYaw, float dRoll) {
    data->lastPitch = data->pitch;
    data->lastYaw = data->yaw;
    data->lastRoll = data->roll;
    data->pitch += dPitch;
    data->yaw += dYaw;
    data->roll += dRoll;
}

void Camera::setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up, bool viewAt) {
    data->mPos = glm::vec3(pos);
    data->mView = glm::vec3(viewAt ? view - pos : view);
    data->mUp = glm::vec3(up);
}

void Camera::setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up) {
    setCameraPosition(pos, view, up, false);
}


void Camera::moveCameraForward(float distance) {
    auto forward = glm::normalize(glm::conjugate(data->qOrientation) * glm::vec3(0, 0, -1));
    data->mPos += forward * distance;
}

void Camera::moveCameraUp(float distance) {
    if (data->trackVerticalAxis) {
        data->mPos += data->mUp * distance;
    } else {
        auto forward = glm::normalize(glm::conjugate(data->qOrientation) * glm::vec3(0, 1, 0));
        data->mPos += forward * distance;
    }
}

void Camera::moveCameraRight(float distance) {
    auto strafe = glm::normalize(glm::conjugate(data->qOrientation) * glm::vec3(1, 0, 0));
    data->mPos += strafe * distance;
}

void Camera::updateLook() {
    CameraImpl::computeLook(this);
    data->mVPMatrix = data->mProjectionMatrix * data->mViewMatrix;
}

glm::mat4 *Camera::getVPMatrix() {
    return &(data->mVPMatrix);
}

const float *Camera::getViewMatrixPtr() {
    return glm::value_ptr(data->mViewMatrix);
}

const float *Camera::getProjectionMatrixPtr() {
    return glm::value_ptr(data->mProjectionMatrix);
}

const float *Camera::getPosition() {
    return (float *) &(data->mPos);
}

const float Camera::getFarZ() {
    return data->farZ;
}

const float Camera::getNearZ() {
    return data->nearZ;
}

const float Camera::getFov() {
    return data->fovy;
}

const float Camera::getAspect() {
    return data->aspect;
}

void Camera::attachTo(GameObject *object) {
    this->attachedTo = object;
}

GameObject *Camera::getAttachedTo() {
    return this->attachedTo;
}

glm::vec3 Camera::getOffsetPos() {
    return this->offset;
}

void Camera::setOffsetPos(glm::vec3 offset) {
    this->offset = offset;
}

void Camera::setPos(glm::vec3 pos) {
    data->mPos = pos;
}

glm::tquat<float> Camera::getQOrientation() {
    return data->qOrientation;
}
