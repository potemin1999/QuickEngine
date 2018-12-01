//
// Created by ilya on 9/24/18.
//

#include "renderer.h"

using namespace QECore;

class RendererImpl::RendererInputReceiver : public InputReceiver {
private:

    Renderer *parent;

public:

    explicit RendererInputReceiver(Renderer *renderer) {
        parent = renderer;
    }

    InputProcessResult onInputEvent(InputEvent *event) override {
        printf("received input action %d with keycode %d\n", event->keyAction, event->keyCode);
    }

};

class RendererImpl::RenderPassContainer {
public:

    const char *name;
    RenderPass *renderPass;

    explicit RenderPassContainer(const char *_name, RenderPass *pass) {
        name = _name;
        renderPass = pass;
    }

};

Renderer::Renderer(EngineContext *_context) {
    pipelineList = new List<RendererImpl::RenderPassContainer *>();
    pipelineListNameMap = new StrMap<int>();
    inputListener = new RendererImpl::RendererInputReceiver(this);
    context = _context;
    //context->getInputEventManager()->registerInputReceiver(inputListener);
    addRenderPass("GeometryPass", new GeometryPass(context));
    addRenderPass("DeferredLightingPass", new DeferredLightingPass(context));
    addRenderPass("SSAOPass", new SSAOPass(context));
    addRenderPass("PostPass", new PostPass(context));
}

Renderer::~Renderer() {
    //context->getInputEventManager()->unregisterInputReceiver(inputListener);
    delete inputListener;
    int size = (int) pipelineList->size();
    for (int i = size - 1; i >= 0; i--) {
        QECore::RendererImpl::RenderPassContainer *passC = (*pipelineList)[i];
        pipelineList->erase(pipelineList->begin() + i);
        pipelineListNameMap->remove(passC->name);
        delete passC->renderPass;
        delete passC;
    }
    delete pipelineList;
    delete pipelineListNameMap;
}

void Renderer::init(RenderDataStorage *storage) {
    renderDataStorage = storage;
    renderDataStorage->set("v_Vignette", &v_Vignette);
    renderDataStorage->set("v_Brightness", &v_Brightness);
    renderDataStorage->set("v_PostBrightness", &v_PostBrightness);
    renderDataStorage->set("v_FXAA", &v_FXAA);
    renderDataStorage->set("v_DrawingMode", &v_DrawingMode);
    renderDataStorage->set("v_Gamma", &v_Gamma);
    renderDataStorage->set("v_SSAOSamples", &v_SSAOSamples);
    renderDataStorage->set("v_PixelSize", &v_PixelSize);
    renderDataStorage->set("v_ScreenSize", &v_ScreenSize);
    renderDataStorage->set("v_SSAOSize", &v_SSAOSize);
    renderDataStorage->set("v_SSAOScale", &v_SSAOScale);
    log("initing %d render passes\n", pipelineList->size());
    for (int i = 0; i < pipelineList->size(); i++) {
        (*pipelineList)[i]->renderPass->init(renderDataStorage);
    }
}


void Renderer::addRenderPass(const char *passName, RenderPass *renderPass) {
    (*pipelineListNameMap)[passName] = pipelineList->size();
    pipelineList->push_back(new RendererImpl::RenderPassContainer(passName, renderPass));
}

void Renderer::removeRenderPass(const char *passName) {
    pipelineList->erase(pipelineList->begin() + (*pipelineListNameMap)[passName]);
    pipelineListNameMap->remove(passName);
}

void Renderer::compileShaders() {
    for (int i = 0; i < pipelineList->size(); i++) {
        (*pipelineList)[i]->renderPass->compileShaders();
    }
}

void Renderer::onResize(int width, int height) {
    v_PixelSize[0] = 1.0f / ((float) (width));
    v_PixelSize[1] = 1.0f / ((float) (height));
    v_ScreenSize[0] = (float) width;
    v_ScreenSize[1] = (float) height;
    v_SSAOSize[0] = (int)(width * 1);
    v_SSAOSize[1] = (int)(height * 1);
    v_SSAOScale[0] = v_SSAOSize[0] / v_ScreenSize[0];
    v_SSAOScale[1] = v_SSAOSize[1] / v_ScreenSize[1];
    for (int i = 0; i < pipelineList->size(); i++) {
        (*pipelineList)[i]->renderPass->onResize(width, height);
    }
}

void Renderer::destroy() {

}

void Renderer::attachWorld(World *_world) {
    world = _world;
}

void Renderer::detachCurrentWorld() {
    world = nullptr;
}

void Renderer::doRender() {
    for (int i = 0; i < pipelineList->size(); i++) {
        (*pipelineList)[i]->renderPass->doDraw();
    }
}



