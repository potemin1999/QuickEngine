#pragma once

#include "glad/glad.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "EngineContext.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"
#include "materials/MaterialManager.h"
#include "util/TextureManager.h"
#include <input/InputEventManager.h>
#include "input/Input.h"
#include <util/Log.h>
#include "Mesh.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

using namespace std;
using namespace QE;


namespace QE {

    class QuickEngine {
    public:

        QuickEngine();

        ~QuickEngine();

        EngineContext *context;
        RenderDataStorage *renderDataStorage;
        Renderer *renderer;
        InputEventManager *inputManager;
        Camera *camera;
        vector<GameObject *> objects;
        vector<Material *> materials;
        vector<QE::Texture *> textures;

        btBroadphaseInterface *broadphaseInterface;
        btDefaultCollisionConfiguration *collisionConfiguration;
        btCollisionDispatcher *collisionDispatcher;
        btSequentialImpulseConstraintSolver *constraintSolver;
        btDiscreteDynamicsWorld *dynamicsWorld;

        int width = 10;
        int height = 10;
        float deltaTime = 0;

        bool lockCursor = true;
        double mouseX, mouseY;
        int windowWidth = 1280, windowHeight = 800;

        void init();

        void initPhysics();

        void compileShaders();

        void resize(int w, int h);

        void tick(float dT);

        void addObject(GameObject *o);

        void dumpUniforms();

        static QuickEngine *getInstance();

        static void checkGlError(const char *tag);

    };


    class EU {
    public:
        static int initUtils(QuickEngine *e);

        static string load_string(char *filename);

        static void genVBOandVAO(float arr[], int len, int &vertVAO);
    };

    class EngineContextImpl : public EngineContext {
    private:

        QuickEngine *engine;

    public:

        explicit EngineContextImpl(QuickEngine *_engine) {
            engine = _engine;
        }

        InputEventManager *getInputEventManager() override {
            return engine->inputManager;
        }

    };

}


