
#include "World.h"
#include <GameObject.h>
#include <World.h>


void QE::World::onCreate() {

}

void QE::World::onDestroy() {

}

void QE::World::addObject(GameObject *object) {
    (*this->objects)[object->getID()] = object;
    if (object->rigidBody != nullptr) {
        dynamicsWorld->addRigidBody(object->rigidBody);
    }
}

void QE::World::removeObject(GameObject *object) {
    this->objects->erase(object->getID());
    if (object->rigidBody != nullptr) {
        dynamicsWorld->removeRigidBody(object->rigidBody);
    }
}

uint64_t World::getID() {
    return id;
}

void World::setID(uint64_t id) {
    this->id = id;
}

World::World(uint64_t id) {
    this->id = id;
    this->objects = new std::map<uint64_t, GameObject *>;
    initPhysics();
}

World::~World() {

}

void World::tick(float dT) {
    for (auto &gameobject : *objects) {
        gameobject.second->tick(dT);
    }

    dynamicsWorld->stepSimulation(dT);
}


void World::initPhysics() {
    broadphaseInterface = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);
    constraintSolver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, broadphaseInterface, constraintSolver,
                                                collisionConfiguration);

    // set default Earth gravitation
    dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
}

std::map<uint64_t, GameObject *> *World::getObjects() {
    return objects;
}
