#include "../public/engine.h"

#define DEBUG_DRAW 1

using namespace QECore;

int supersampling = 1;
int anisotropy = 16;


void Engine::checkGlError(const char *tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("ERROR %s : %i \n description: no \n", tag, error);
    }
}

Engine::Engine() {
    log("created new engine\n");
    inputManager = new InputEventManager();
    context = new EngineContextImpl(this);
    renderDataStorage = new RenderDataStorage();
    renderer = new Renderer(context);
}

Engine::~Engine() {
    log("deleting engine\n");
    delete renderer;
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

}

void Engine::init() {
    int i_eu = EU::init_utils(this);
    log("init engine utils %i \n", i_eu);
    camera = new Camera();

    auto p = glm::vec3(0.0f, 0.0f, 0.0f);
    auto v = glm::vec3(0.0f, 0.0f, 1.0f);
    auto u = glm::vec3(0.0f, 1.0f, 0.0f);
    camera->setCameraPosition(p, v, u);
    camera->setTrackVerticalAxis(true);

    renderDataStorage->set("с_CurrentWorldCamera",camera);
    renderDataStorage->set("objects",&objects);

    renderer->init(renderDataStorage);
}


void Engine::compileShaders() {
    log("shader compiling started\n");
    renderer->compileShaders();
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
    camera->setPerspective(((float) w) / ((float) h), 1.0f, 4000.0f);
    renderer->onResize(w,h);
    if (supersampling != 1) {
        w /= supersampling;
        h /= supersampling;
    }
}

void Engine::draw(float dT) {
    inputManager->processDelayedEvents();
    if (request_forward & !request_backward) camera->moveCameraForward(forward_speed * dT);
    if (request_backward & !request_forward) camera->moveCameraForward(-backward_speed * dT);
    if (request_right & !request_left) camera->moveCameraRight(strafe_speed * dT);
    if (request_left & !request_right) camera->moveCameraRight(-strafe_speed * dT);
    if (request_up & !request_down) camera->moveCameraUp(2*dT);
    if (request_down & !request_up) camera->moveCameraUp(-dT*2);
    camera->updateLook();
    renderer->doRender();
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
