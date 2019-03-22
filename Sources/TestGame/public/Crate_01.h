#pragma once

#include "GameObject.h"
#include "ModelLoader.h"

class Crate_01 : public GameObject {
public:
   explicit  Crate_01(World *world) : GameObject(world) {
        this->model = ModelLoader::load_object("", "crate_01.obj");

        glm::tquat rot = this->getRotation();
        btQuaternion rotation(rot.x, rot.y, rot.z, rot.w);

        glm::vec3 pos = this->getPos();
        btVector3 position = btVector3(pos.x, pos.y, pos.z);

        // init construction info here
        btDefaultMotionState *motionState = new btDefaultMotionState(btTransform(rotation, position));
        btCollisionShape *shape = new btBoxShape(btVector3(0.5, 0.5, 0.5));

        btScalar bodyMass = 1;
        btVector3 bodyInertia;
        shape->calculateLocalInertia(bodyMass, bodyInertia);

        btRigidBody::btRigidBodyConstructionInfo info(bodyMass, motionState, shape, bodyInertia);

        info.m_restitution = 0.5f;
        info.m_friction = 0.5f;

        rigidBody = new btRigidBody(info);
    };
};