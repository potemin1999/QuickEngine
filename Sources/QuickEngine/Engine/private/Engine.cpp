
#include "Engine.h"
#include <chrono>
#include <vector>

#define DEBUG_DRAW 1

using namespace QE;

int supersampling = 1;
int anisotropy = 16;


void QuickEngine::checkGlError(const char *tag) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("ERROR %s : %i \n description: no \n", tag, error);
    }
}

QuickEngine::QuickEngine() {
    log("created new engine\n");
    inputManager = new InputEventManager();
    context = new EngineContextImpl(this);
    renderDataStorage = new RenderDataStorage();
    renderer = new Renderer(context);
}

QuickEngine::~QuickEngine() {
    log("deleting engine\n");
    delete renderer;
    delete inputManager;
    delete context;
    delete camera;

    for (auto &world : worlds) {
        worlds.erase(world.first);
        delete world.second;
    }
    for (auto &material : materials) {
        delete material;
    }
    for (auto &texture : textures) {
        delete texture;
    }

}

void QuickEngine::init() {
    int i_eu = EU::initUtils(this);
    log("init engine utils %i \n", i_eu);

    printf("initialising camera...\n");
    camera = new Camera();
    renderDataStorage->set("с_CurrentWorldCamera", camera);
    printf("camera initialized.\n");

    renderer->init(renderDataStorage);
    printf("engine initialized.\n");
}


void QuickEngine::compileShaders() {
    log("shader compiling started\n");
    renderer->compileShaders();
    log("shader program binded\n");
//#ifdef GRAPHICS_DEBUG
    dumpUniforms();
//#endif
}

void QuickEngine::dumpUniforms() {

}

void QuickEngine::resize(int w, int h) {
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
    renderer->onResize(w, h);
    if (supersampling != 1) {
        w /= supersampling;
        h /= supersampling;
    }
}

void QuickEngine::tick(float dT) {
    this->deltaTime = dT;

    inputManager->processDelayedEvents();

    for (auto &world : worlds) {
        world.second->tick(dT);
    }

    camera->updateLook();
    renderer->doRender();
}


QuickEngine *QuickEngine::getInstance() {
    static QuickEngine *engine;
    if (!engine) {
        engine = new QuickEngine();
    }
    return engine;
}

void QuickEngine::addWorld(World *world) {
    this->worlds[world->getID()] = world;
}

World *QuickEngine::getWorld(uint64 id) {
    return this->worlds[id];
}

void QuickEngine::removeWorld(World *world) {
    this->worlds.erase(world->getID());
}

void QuickEngine::removeWorld(uint64 id) {
    this->worlds.erase(id);
}
