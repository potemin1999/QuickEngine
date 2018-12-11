#pragma once

#include "defines.h"
#include "GameObject.h"
#include "ModelLoader.h"

class FloorModel : public GameObject {
public:
    FloorModel() : GameObject() {
        ModelLoader::load_object(this, "", "floor_01.obj");

        btQuaternion rotation;
        glm::vec3 rot = this->getRotation();
        rotation.setEulerZYX(rot.x, rot.y, rot.z);

        glm::vec3 pos = this->getPos();
        btVector3 position = btVector3(pos.x, pos.y, pos.z);

        // init construction info here
        btDefaultMotionState *motionState = new btDefaultMotionState(btTransform(rotation, position));
        btCollisionShape *shape = new btBoxShape(btVector3(100, 0, 100));

        btScalar bodyMass = 0;
        btVector3 bodyInertia;
        shape->calculateLocalInertia(bodyMass, bodyInertia);

        btRigidBody::btRigidBodyConstructionInfo info(bodyMass, motionState, shape, bodyInertia);

        info.m_restitution = 0.1f;
        info.m_friction = 0.5f;

        rigidBody = new btRigidBody(info);
    };
};