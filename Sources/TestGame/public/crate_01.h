#pragma once

#include "defines.h"
#include "gameobject.h"
#include "model_loader.h"

class Crate_01 : public GameObject {
public:
    Crate_01() : GameObject() {
        ModelLoader::load_object(this, "", "crate_01.obj");

        btQuaternion rotation;
        glm::vec3 rot = this->getRotation();
        rotation.setEulerZYX(rot.x, rot.y, rot.z);

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