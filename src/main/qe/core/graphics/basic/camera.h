
#ifndef CAMERA
#define CAMERA

#include "qe/core/graphics/defines.h"
#include <qe/core/util/log.h>
#include <stdio.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


/*
 *  header file of camera.cpp
 */

namespace QEGraphics {
    class Camera {


    public:

/*
 * the first and the last constructor
 */

        Camera();


        float aspect;
        float fovy = 2.0f;
        float farZ = 40.0f;
        float nearZ = 0.1f;
        const float PI = 3.1415927f;


        void updatePerspectiveMatrix();

        void setPerspective(float aspect, float fovy, float distance);

        void set_aspect(float aspect);


    public:

        /**
        * camera position
        */
        vec3 mPos;

        /**
        * normalized vector of view
        */
        vec3 mView;

        /**
        * up vector
        */
        vec3 mUp;


        /**
        * rotation of view in radians
        * [0] +right     -left
        * [1] +up        -down
        * [2] +clockwise -counterclockwise
        */
        float mRotation[3];

        /**
        * right vector
        */
        vec3 mStrafe;

        /**
        * matrix of view
        */
        mat4 mViewMatrix;

        /**
        * matrix to project vertices
        */
        mat4 mProjectionMatrix;

        /**
        * matrix of model
        */
        mat4 mModelMatrix;

        /**
        * matrix = [ model × view ] × projection
        */
        mat4 mVPMatrix;

        /**
        * matrix = [ model × view ]
        */
        mat4 mVMatrix;


    public:


/*
 * calls the need of recompute view vector is
 */
        void look();


/* writes in [matrix]
* [ 1.0 , 0.0 , 0.0 , 0.0 ]
* [ 0.0 , 1.0 , 0.0 , 0.0 ]
* [ 0.0 , 0.0 , 1.0 , 0.0 ]
* [ 0.0 , 0.0 , 0.0 , 1.0 ]
*/
        static void setIdentity(mat4 *matrix);

/*
 * set camera position, view vector and up vector
 */

        void positionCamera(vec3 *pos, vec3 *view, vec3 *up);

        void position_camera(float pos[], float view[], float up[]) {
            vec3 vPos(pos[0], pos[1], pos[2]);
            vec3 vView(view[0], view[1], view[2]);
            vec3 vUp(up[0], up[1], up[2]);
            positionCamera(&vPos, &vView, &vUp);
        }

/*
 * set camera position
 */
        void position_camera(vec3 *pos);

/*
 * move camera forward on distance
 */
        void moveCamera(float distance);

/*
 * move camera up on distance
 */
        void moveCameraUp(float distance);

/*
 * move camera right on distance
 */
        void moveStrafe(float distance);

/*
 * rotates view up on angle in radians
 */
        void rotateViewUp(float angle);

/*
 * rotates view right on angle in radians
 */
        void rotateViewRight(float angle);


    public:

/*
 * returns tangens of angle in degrees
 */
//static float tan(float degrees);

/*
 * returns sin of angle in radians
 */
//static float sin(float rad);

/*
 * returns cos of angle in radians
 */
//static float cos(float rad);

/*
 * returns 1.0 / square root of x
 */
        static float invSqrt(float x);

/*
 * result = [ v × v3 ]
 */
//static void cross(vec3* result, vec3* v, vec3* v3);

/*
 * returns length of vec3
 */
//static float length(vec3* vec3);

/*
 * return length of vec3
 */
//static float magnitude(vec3* vec3);

/*
 * sets length of vec3 to 1.0
 */
        static void normalize(vec3 *vec3);

/*
 * translates [matrix] on vec3
 */
//static void translate(mat4* matrix, vec3* vec3);

/*
 * translates [matrix] on vec3(x,y,z)
 */
//static void translate(mat4* matrix,float x,float y,float z);

/*
 * sets [matrix] to projection matrix with parameters
 */
//static void perspective(mat4* matrix,float fovy,float aspect,float zNear,float zFar);


    private:


/*
 * updates strafe vector
 */
        void updateStrafe();

    public:

        float *value_ptr(mat4 m) {
            return glm::value_ptr(m);
        }

/*
 * computes [view_matrix] by eye position, up vector and mRotation angles
 */
        void computeLook(mat4 *view_matrix);


    };

}


#endif




