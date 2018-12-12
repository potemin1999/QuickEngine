#include "Terrain.h"
#include "ModelLoader.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>

Terrain::Terrain(World *world) : GameObject(world) {


//    ModelLoader::load_object(this, "", "floor_01.obj");
//
//    glm::tquat rot = this->getRotation();
//    btQuaternion rotation(rot.x, rot.y, rot.z, rot.w);
//
//    glm::vec3 pos = this->getPos();
//    btVector3 position = btVector3(pos.x, pos.y, pos.z);
//
//    // init construction info here
//    btDefaultMotionState *motionState = new btDefaultMotionState(btTransform(rotation, position));
//    btCollisionShape *shape = new btHeightfieldTerrainShape();
//
//    btScalar bodyMass = 0;
//    btVector3 bodyInertia;
//    shape->calculateLocalInertia(bodyMass, bodyInertia);
//
//    btRigidBody::btRigidBodyConstructionInfo info(bodyMass, motionState, shape, bodyInertia);
//
//    info.m_restitution = 0.1f;
//    info.m_friction = 0.5f;
//
//    rigidBody = new btRigidBody(info);
}
