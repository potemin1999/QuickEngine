//
// Created by ilya on 9/26/18.
//

#include <GameObject.h>

#include "GameObject.h"

GameObject::GameObject(World *world) {
    this->world = world;
}

GameObject::~GameObject() {
    mesh_count = 0;
    delete meshes;
}

void GameObject::setPos(glm::vec3 pos) {
    this->pos = pos;

    if (rigidBody) {
        btTransform transform = rigidBody->getWorldTransform();
        transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
        // TODO: check if it is possible to remove this assignment
        rigidBody->setWorldTransform(transform);
    }
}

glm::vec3 GameObject::getPos() {
    if (rigidBody) {
        btTransform transform = rigidBody->getWorldTransform();
        btVector3 pos = transform.getOrigin();
        return glm::vec3(pos.x(), pos.y(), pos.z());
    } else {
        return pos;
    }
}

void GameObject::setRotation(glm::tquat<float> rot) {
    // TODO: check if it works
    this->rot = rot;
    if (rigidBody) {
        rigidBody->getWorldTransform().setRotation(btQuaternion(rot.x, rot.y, rot.z, rot.w));
    }
}

glm::tquat<float> GameObject::getRotation() {
    // TODO: check if it works
    if (rigidBody) {
        btQuaternion rotation = rigidBody->getWorldTransform().getRotation();
        return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
    } else
        return rot;
}

float GameObject::getMass() {
    return mass;
}

// TODO: check why it crashes engine
void GameObject::setMass(float mass) {
    this->mass = mass;
    btVector3 bodyInertia;
    rigidBody->getCollisionShape()->calculateLocalInertia(mass, bodyInertia);
    rigidBody->setMassProps(mass, bodyInertia);
}

void GameObject::move(glm::vec3 offset) {
    setPos(getPos() + offset);
}

glm::mat4 GameObject::getModelMatrix() {
    glm::mat4 m(0);
    const btMatrix3x3 &basis = rigidBody->getWorldTransform().getBasis();
    // rotation
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            m[c][r] = basis[r][c];
        }
    }
    // translation
    btVector3 origin = rigidBody->getWorldTransform().getOrigin();
    m[3][0] = origin.getX();
    m[3][1] = origin.getY();
    m[3][2] = origin.getZ();
    // unit scale
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
    return m;

}

uint64 GameObject::getID() {
    return this->id;
}

World *GameObject::getWorld() {
    return this->world;
}