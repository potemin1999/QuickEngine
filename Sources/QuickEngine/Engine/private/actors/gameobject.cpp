//
// Created by ilya on 9/26/18.
//

#include <gameobject.h>

#include "gameobject.h"

Object::Object() {

}

Object::~Object() {
    mesh_count = 0;
    delete meshes;
}

void Object::setPos(glm::vec3 pos) {
    this->pos = pos;

    if (rigidBody) {
        btTransform transform = rigidBody->getWorldTransform();
        transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
        // TODO: check if it is possible to remove this assignment
        rigidBody->setWorldTransform(transform);
    }
}

glm::vec3 Object::getPos() {
    if (rigidBody) {
        btTransform transform = rigidBody->getWorldTransform();
        btVector3 pos = transform.getOrigin();
        return glm::vec3(pos.x(), pos.y(), pos.z());
    } else {
        return pos;
    }
}

void Object::setRotation(glm::vec3 rot) {
    // TODO: implement
//    this->rot = rot;
}

glm::vec3 Object::getRotation() {
    // TODO: implement
    return rot;
}

float Object::getMass() {
    return mass;
}

// TODO: check why it crashes engine
void Object::setMass(float mass) {
    this->mass = mass;
    btVector3 bodyInertia;
    rigidBody->getCollisionShape()->calculateLocalInertia(mass, bodyInertia);
    rigidBody->setMassProps(mass, bodyInertia);
}