#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include <btBulletDynamicsCommon.h>

using namespace glm;

namespace QE {

    class GameObject {
    private:
        float mass = 0;
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::tquat<float> rot;
    public:

        GameObject();

        ~GameObject();

        Mesh *meshes;
        unsigned long mesh_count;
        btRigidBody *rigidBody = nullptr;

        void setPos(glm::vec3 pos);

        glm::vec3 getPos();

        void setRotation(glm::tquat<float> rot);

        glm::tquat<float> getRotation();

        float getMass();

        void setMass(float mass);

        void move(glm::vec3 offset);

        glm::mat4 getModelMatrix();
    };

}
