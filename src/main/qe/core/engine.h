#pragma once

#include "glad/glad.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "qe/core/engine_context.h"
#include "qe/core/actors/object.h"
#include <qe/core/graphics/defines.h>
#include <qe/core/graphics/renderer.h>
#include <qe/core/graphics/basic/camera.h>
#include <qe/core/graphics/basic/shader.h>
#include <qe/core/graphics/basic/texture.h>
#include <qe/core/graphics/geometry/geometry_pass.h>
#include <qe/core/graphics/effects/post_pass.h>
#include <qe/core/graphics/lighting/deferred_lighing_pass.h>
#include <qe/core/graphics/lighting/ssao_pass.h>
#include <qe/core/graphics/materials/material_manager.h>
#include <qe/core/graphics/util/texture_manager.h>
#include <qe/core/input/input_event_manager.h>
#include <qe/core/input/input_key.h>
#include <qe/core/util/log.h>
#include "qe/core/actors/mesh.h"

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

        void compileShaders();

        void createWorld();

        void resize(int w, int h);

        void draw(float dT);

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

        static void create_objects(Engine *e);
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


