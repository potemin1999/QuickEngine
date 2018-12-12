#pragma once

#include <map>
#include "GameObject.h"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

using namespace QE;

namespace QE {
    class GameObject;

    class World {
    private:
        uint64_t id;
        std::map<uint64_t, GameObject *> *objects;

        btBroadphaseInterface *broadphaseInterface;
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *collisionDispatcher;
        btSequentialImpulseConstraintSolver *constraintSolver;
        btDiscreteDynamicsWorld *dynamicsWorld;

    public:

        World(uint64_t id);

        ~World();

        void onCreate();

        void onDestroy();

        void addObject(GameObject *object);

        void removeObject(GameObject *object);

        void setID(uint64_t id);

        void tick(float dT);

        uint64_t getID();

        void initPhysics();

        std::map<uint64_t, GameObject *> *getObjects();
    };

}