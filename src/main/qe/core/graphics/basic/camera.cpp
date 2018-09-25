/*
 *  Only for personal usage
 */

#include "camera.h"

using namespace QEGraphics;


int FSIA = 1;
float piD2 = (float) (3.14159f / 2.0f);
float sTemp[32];
float average_look_time = 0;


long look_count = 1;


Camera::Camera() {
    mRotation[0] = 0;
    mRotation[1] = 0;
    mRotation[2] = 0;
    mUp = vec3(0, 1, 0);
}


void Camera::updatePerspectiveMatrix() {
    setIdentity(&mProjectionMatrix);
    mProjectionMatrix = perspective(fovy, aspect, nearZ, farZ);

    log("perspective matrix updated aspect %f  fovy %f  distance %f\n", aspect, fovy, farZ);
}

void Camera::setPerspective(float aspect, float fovy, float distance) {
    Camera::aspect = aspect;
    Camera::fovy = fovy;
    Camera::farZ = distance;
    Camera::nearZ = 0.01f;
    updatePerspectiveMatrix();
}

void Camera::set_aspect(float aspect) {
    Camera::aspect = aspect;
    updatePerspectiveMatrix();
}


void Camera::positionCamera(vec3 *pos, vec3 *view, vec3 *up) {
    mPos.x = pos->x;
    mPos.y = pos->y;
    mPos.z = pos->z;
    mView.x = view->x - pos->x;
    mView.x = view->y - pos->y;
    mView.z = view->z - pos->z;
    mUp.x = up->x;
    mUp.y = up->y;
    mUp.z = up->z;
    normalize(&mView);
    log("set  position x:%f y:%f z:%f\n", mPos.x, mPos.y, mPos.z);
    log("set   look at x:%f y:%f z:%f\n", mView.x, mView.y, mView.z);
    log("set up vector x:%f y:%f z:%f\n", mUp.x, mUp.y, mUp.z);
}


float Camera::invSqrt(float x) {
    float half = x * 0.5f;
    int i = *(int *) &x;
    i = 0x5f3759df - (i >> 1);
    x = *(float *) &i;
    x = x * (1.5f - half * x * x);
    return x;
}


/*
 *  length of v gets to 1.0f
 */


void Camera::normalize(vec3 *v) {
    float m = invSqrt(v->length());
    v->x = v->x * m;
    v->y = v->y * m;
    v->z = v->z * m;
}


void Camera::position_camera(vec3 *pos) {
    mPos.x = pos->x;
    mPos.y = pos->y;
    mPos.z = pos->z;
}


void Camera::rotateViewRight(float angle) {
    vec3 *v = new vec3[3];
    v->x = mView.x;//- mPos[0];
    v->y = mView.y;//- mPos[1];
    v->z = mView.z;//- mPos[2];
    mView.x = (float) (cos(-angle) * v->x - sin(-angle) * v->z);
    mView.z = (float) (sin(-angle) * v->x + cos(-angle) * v->z);
    mRotation[0] += angle;
    delete v;
}


void Camera::rotateViewUp(float angle) {
    mRotation[1] += angle;
    mView.y = mRotation[1];
}


void Camera::moveCamera(float distance) {
    vec3 v;
    v.x = mView.x;
    v.y = mView.y;
    v.z = mView.z;
    normalize(&v);
    mPos.x = mPos.x + v.x * distance;
    mPos.z = mPos.z + v.z * distance;
}

void Camera::moveCameraUp(float distance) {
    mPos += mUp * distance;
}

void Camera::moveStrafe(float distance) {
    updateStrafe();
    mPos += mStrafe * distance;
    //mPos.x = mPos.x + mStrafe.x * distance;
    //mPos.z = mPos.z + mStrafe.z * distance;
}

void Camera::updateStrafe() {
    mStrafe = cross(mView, mUp);
    //cross(mStrafe,mView,mUp);
    normalize(&mStrafe);
}


void Camera::look() {
    computeLook(&mViewMatrix);
    mVMatrix;
    mVPMatrix = mProjectionMatrix * mViewMatrix;
}


void Camera::setIdentity(mat4 *f) {
    //(&f)[0][0] = 1.0f;

}


void Camera::computeLook(mat4 *m) {
    if (mRotation[1] >= piD2) mRotation[1] = piD2;
    if (mRotation[1] <= -piD2) mRotation[1] = -piD2;
    float horAR = mRotation[0];
    float vertAR = mRotation[1];
    float zAR = mRotation[2];
    vec3 dir = vec3(cos(vertAR) * sin(horAR), sin(vertAR), cos(vertAR) * cos(horAR));
    mViewMatrix = lookAt(mPos, mPos + dir, mUp);
    mVMatrix = lookAt(vec3(0.0), dir, mUp);
}
	
