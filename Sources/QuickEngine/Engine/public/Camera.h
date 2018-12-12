#pragma once

#include <glm/gtx/quaternion.hpp>
#include "GameObject.h"

namespace QE {

    class CameraData;

    class Camera {

    private:
        GameObject *attachedTo;
        glm::vec3 offset = glm::vec3(0, 0, 0);

    public:

        Camera();

        ~Camera();

        CameraData *data;

        void setPerspective(float aspect, float fovy, float farZ);

        static float invSqrt(float x);

        /*
         * calls the need of recompute view vector is
         */
        void updateLook();

        void setTrackVerticalAxis(bool track);

        void setCameraRotation(float pitch, float yaw, float roll);

        void addCameraRotation(float dPitch, float dYaw, float dRoll);

        /*
         * set camera position, view vector and up vector
         */

        void setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up, bool viewAt);

        void setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up);

        void setPos(glm::vec3 pos);

        /*
         * move camera forward by distance
         */
        void moveCameraForward(float distance);

        /*
         * move camera up by distance
         */
        void moveCameraUp(float distance);

        /*
         * move camera right by distance
         */
        void moveCameraRight(float distance);

        glm::vec3 getForward();

        glm::mat4 *getVPMatrix();

        const float *getViewMatrixPtr();

        const float *getProjectionMatrixPtr();

        const float *getPosition();

        const float getFarZ();

        const float getNearZ();

        const float getFov();

        const float getAspect();

        void attachTo(GameObject *object);

        GameObject *getAttachedTo();

        glm::vec3 getOffsetPos();

        void setOffsetPos(glm::vec3 offset);

        glm::tquat<float> getQOrientation();
    };

}

