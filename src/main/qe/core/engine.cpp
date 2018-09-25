#include "engine.h"

#define DEBUG_DRAW 1

using namespace QECore;

int supersampling = 1;
int anisotropy = 8;


void Engine::checkGlError(const char *tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("ERROR %s : %i \n description: no \n", tag, error);
    }
}

Engine::Engine() {
    log("created new engine\n");
    inputManager = new InputEventManager();
    p_GeometryPass = new GeometryPass();
    p_DeferredLightingPass = new DeferredLightingPass();
    p_SSAOPass = new SSAOPass();
    p_PostPass = new PostPass();
    context = new EngineContextImpl(this);
    renderer = new Renderer(context);
}

Engine::~Engine() {
    log("deleting engine\n");
    delete renderer;
    delete p_PostPass;
    delete p_SSAOPass;
    delete p_DeferredLightingPass;
    delete p_GeometryPass;
    delete inputManager;
    delete context;

    delete camera;

    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
    for (int i = 0; i < materials.size(); i++) {
        delete materials[i];
    }
    for (int i = 0; i < textures.size(); i++) {
        delete textures[i];
    }

    delete v_PixelSize;
    delete v_ScreenSize;
    delete v_SSAOScale;
    delete v_SSAOSize;
    delete v_SSAOSamples;
    delete v_Brightness;
    delete v_PostBrightness;
    delete v_DrawingMode;
    delete v_FXAA;
    delete v_Gamma;
}

void Engine::init() {
    int i_eu = EU::init_utils(this);
    log("init engine utils %i \n", i_eu);
    camera = new Camera();
    camera->farZ = 10.0f;
    camera->nearZ = 0.01f;
    camera->fovy = 30.14159f / 3.0f;
    float p[] = {0.0f, 0.0f, 0.0f};
    float v[] = {0.0f, 0.0f, 1.0f};
    float u[] = {0.0f, 1.0f, 0.0f};
    camera->position_camera(p, v, u);

    *v_Vignette = 1.0f;
    *v_Brightness = 1.0f;
    *v_PostBrightness = 1.0f;
    *v_FXAA = 1;
    *v_DrawingMode = 0;
    *v_Gamma = 1.0;
    *v_SSAOSamples = 32;

    GeometryPassInitData geometryPassInitData;
    geometryPassInitData.objects = &objects;
    geometryPassInitData.camera = &camera;
    geometryPassInitData.v_Brightness = &v_Brightness;
    p_GeometryPass->init(&geometryPassInitData);

    DeferredLightingPassInitData deferredLightingPassInitData;
    deferredLightingPassInitData.v_PixelSize = &v_PixelSize;
    deferredLightingPassInitData.t_PositionTex = p_GeometryPass->getPositionTex();
    deferredLightingPassInitData.t_NormalTex = p_GeometryPass->getNormalTex();
    deferredLightingPassInitData.t_AlbedoTex = p_GeometryPass->getAlbedoTex();
    p_DeferredLightingPass->init(&deferredLightingPassInitData);

    SSAOPassInitData ssaoPassInitData;
    ssaoPassInitData.v_SSAOScale = &v_SSAOScale;
    ssaoPassInitData.v_PixelSize = &v_PixelSize;
    ssaoPassInitData.v_SSAOSamples = &v_SSAOSamples;
    ssaoPassInitData.camera = &camera;
    ssaoPassInitData.t_Depthmap = p_GeometryPass->getDepthmap();
    ssaoPassInitData.t_LightingTex = p_GeometryPass->getPositionTex();
    ssaoPassInitData.t_PositionTex = p_GeometryPass->getPositionTex();
    ssaoPassInitData.t_NormalTex = p_GeometryPass->getNormalTex();
    p_SSAOPass->init(&ssaoPassInitData);

    PostPassInitData postPassInitData;
    postPassInitData.v_PixelSize = &v_PixelSize;
    postPassInitData.v_ScreenSize = &v_ScreenSize;
    postPassInitData.v_Gamma = &v_Gamma;
    postPassInitData.v_FXAA = &v_FXAA;
    postPassInitData.v_Vignette = &v_Vignette;
    postPassInitData.v_PostBrightness = &v_PostBrightness;
    postPassInitData.v_DrawingMode = &v_DrawingMode;
    postPassInitData.t_AlbedoTex = p_GeometryPass->getAlbedoTex();
    postPassInitData.t_NormalTex = p_GeometryPass->getNormalTex();
    postPassInitData.t_PositionTex = p_GeometryPass->getPositionTex();
    postPassInitData.t_OcclusionTex = p_SSAOPass->getOcclusionTexture();
    p_PostPass->init(&postPassInitData);
}


void Engine::compileShaders() {
    log("shader compiling started\n");
    p_GeometryPass->compileShaders();
    p_DeferredLightingPass->compileShaders();
    p_SSAOPass->compileShaders();
    p_PostPass->compileShaders();
    log("shader program binded\n");
//#ifdef GRAPHICS_DEBUG
    dumpUniforms();
//#endif
}

void Engine::dumpUniforms() {

}

void Engine::createWorld() {
    EU::create_objects(this);
}

void Engine::resize(int w, int h) {
    if (h == 0) h = 10;
    if (w == 0) w = 10;
    width = w;
    height = h;
    glViewport(0, 0, width, height);
    if (supersampling != 1) {
        w *= supersampling;
        h *= supersampling;
    }
    camera->setPerspective(((float) w) / ((float) h), 1.0f, 40.0f);
    int m_offset = sizeof(unsigned);
    v_PixelSize[0] = 1.0f / ((float) (w));
    v_PixelSize[1] = 1.0f / ((float) (h));
    v_ScreenSize[0] = (float) w;
    v_ScreenSize[1] = (float) h;
    v_SSAOSize[0] = w;
    v_SSAOSize[1] = h;
    v_SSAOScale[0] = v_ScreenSize[0] / v_SSAOSize[0];
    v_SSAOScale[1] = v_ScreenSize[1] / v_SSAOSize[1];

    p_GeometryPass->onResize(w, h);
    p_DeferredLightingPass->onResize(w, h);
    p_SSAOPass->onResize(w, h);
    p_PostPass->onResize(w, h);

    if (supersampling != 1) {
        w /= supersampling;
        h /= supersampling;
    }
}

void Engine::draw(float dT) {
    if (request_forward & !request_backward) camera->moveCamera(forward_speed * dT);
    if (request_backward & !request_forward) camera->moveCamera(-backward_speed * dT);
    if (request_right & !request_left) camera->moveStrafe(strafe_speed * dT);
    if (request_left & !request_right) camera->moveStrafe(-strafe_speed * dT);
    if (request_up & !request_down) camera->moveCameraUp(dT);
    if (request_down & !request_up) camera->moveCameraUp(-dT);
    camera->look();

    p_GeometryPass->doDraw();
    p_DeferredLightingPass->doDraw();
    p_SSAOPass->doDraw();
    p_PostPass->doDraw();

    glFlush();
}


void Engine::addObject(Object *o) {
    objects.push_back(o);
}

Engine *Engine::getInstance() {
    static Engine *engine;
    if (!engine) {
        engine = new Engine();
    }
    return engine;
}
