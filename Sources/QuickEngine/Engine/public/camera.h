
#ifndef CAMERA
#define CAMERA

#include <glm/gtx/quaternion.hpp>

namespace QECore {

    class CameraData;

    class Camera {

    public:

        Camera();

        ~Camera();

        CameraData *data;

        void setPerspective(float aspect, float fovy, float farZ);

    public:

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

        void setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up,bool viewAt);

        void setCameraPosition(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up);

        /*
         * move camera forward on distance
         */
        void moveCameraForward(float distance);

        /*
         * move camera up on distance
         */
        void moveCameraUp(float distance);

        /*
         * move camera right on distance
         */
        void moveCameraRight(float distance);


        glm::mat4 *getVPMatrix();

        const float *getViewMatrixPtr();

        const float *getProjectionMatrixPtr();

        const float *getPosition();

        const float getFarZ();

        const float getNearZ();

        const float getFov();

        const float getAspect();


    };

}


#endif




