#pragma once

#include "glad/glad.h"
#include <list>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "engine_context.h"
#include "object.h"
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
#include "mesh.h"

using namespace std;
using namespace QEInput;


namespace QECore {

    class Engine {
    public:
        Engine();

        ~Engine();

        EngineContext *context;
        Renderer *renderer;
        InputEventManager *inputManager;
        Camera *camera;
        vector<Object *> objects;
        vector<Material *> materials;
        vector<QEGraphics::Texture *> textures;

        GeometryPass *p_GeometryPass;
        DeferredLightingPass *p_DeferredLightingPass;
        SSAOPass *p_SSAOPass;
        PostPass *p_PostPass;

        //unsigned int* mFBO_tex = new unsigned[2];

        float *v_PixelSize = new float[2];
        float *v_ScreenSize = new float[2];
        float *v_SSAOScale = new float[2];
        int *v_SSAOSize = new int[2];
        int *v_SSAOSamples = new int;

        float *v_Vignette = new float;
        float *v_Brightness = new float;
        float *v_PostBrightness = new float;
        int *v_DrawingMode = new int;
        int *v_FXAA = new int;
        float *v_Gamma = new float;


        int width = 10;
        int height = 10;
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

    private:
        float speed_up = 0;
        float speed_forward = 0;
        float speed_right = 0;
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
        EngineContextImpl(Engine *_engine) { engine = _engine; }

        InputEventManager *getInputEventManager() { return engine->inputManager; }
    };

}


