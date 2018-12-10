#pragma once

#include "glad/glad.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine_context.h"
#include "gameobject.h"
#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "materials/material_manager.h"
#include "util/texture_manager.h"
#include <input/input_event_manager.h>
#include "input/input_key.h"
#include <util/log.h>
#include "mesh.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

using namespace std;
using namespace QECore;


namespace QECore {

    class Engine {
    public:

        Engine();

        ~Engine();

        EngineContext *context;
        RenderDataStorage *renderDataStorage;
        Renderer *renderer;
        InputEventManager *inputManager;
        Camera *camera;
        vector<Object *> objects;
        vector<Material *> materials;
        vector<QECore::Texture *> textures;

        btBroadphaseInterface *broadphaseInterface;
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *collisionDispatcher;
        btSequentialImpulseConstraintSolver *constraintSolver;
        btDiscreteDynamicsWorld *dynamicsWorld;

        int width = 10;
        int height = 10;
        int anisotropy = 8;
        float forward_speed = 3.8f;
        float backward_speed = 2.8f;
        float strafe_speed = 3.2f;

        bool request_up = false;
        bool request_down = false;
        bool request_left = false;
        bool request_right = false;
        bool request_forward = false;
        bool request_backward = false;

        void init();

        void initPhysics();

        void compileShaders();

        void resize(int w, int h);

        void tick(float dT);

        void addObject(Object *o);

        void dumpUniforms();

        static Engine *getInstance();

        static void checkGlError(const char *tag);

    };


    class EU {
    public:
        static int init_utils(Engine *e);

        static string load_string(char *filename);

        static void genVBOandVAO(float arr[], int len, int &vertVAO);
    };

    class EngineContextImpl : public EngineContext {
    private:

        Engine *engine;

    public:

        explicit EngineContextImpl(Engine *_engine) {
            engine = _engine;
        }

        InputEventManager *getInputEventManager() override {
            return engine->inputManager;
        }

    };

}


