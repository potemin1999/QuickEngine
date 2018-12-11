//
// Created by ilya on 9/26/18.
//

#include <gameobject.h>

#include "gameobject.h"

GameObject::GameObject() {

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

void GameObject::setRotation(glm::vec3 rot) {
    // TODO: implement
//    this->rot = rot;
}

glm::vec3 GameObject::getRotation() {
    // TODO: implement
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
