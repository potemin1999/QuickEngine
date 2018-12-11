#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"
#include <btBulletDynamicsCommon.h>

using namespace glm;

namespace QECore {

    class GameObject {
    private:
        float mass = 0;
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::vec3 rot = glm::vec3(0, 0, 0);
    public:

        GameObject();

        ~GameObject();

        Mesh *meshes;
        unsigned long mesh_count;
        btRigidBody *rigidBody = nullptr;

        void setPos(glm::vec3 pos);

        glm::vec3 getPos();

        void setRotation(glm::vec3 rot);

        glm::vec3 getRotation();

        float getMass();

        void setMass(float mass);

        void move(glm::vec3 offset);
    };

}
