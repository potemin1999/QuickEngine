#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "World.h"
#include <btBulletDynamicsCommon.h>
#include "Model.h"


namespace QE {
    class World;

    class GameObject {
    private:
        uint64_t id;
        float mass = 0;
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::tquat<float> rot;
        World *world;

    public:

        GameObject(World *world);

        ~GameObject();

//        Mesh *meshes;
//        unsigned long mesh_count;
        Model *model = nullptr;
        btRigidBody *rigidBody = nullptr;

        void setPos(glm::vec3 pos);

        glm::vec3 getPos();

        void setRotation(glm::tquat<float> rot);

        glm::tquat<float> getRotation();

        float getMass();

        void setMass(float mass);

        void move(glm::vec3 offset);

        glm::mat4 getModelMatrix();

        uint64_t getID();

        void tick(float dT) {};

        World *getWorld();
    };

}

using namespace glm;
