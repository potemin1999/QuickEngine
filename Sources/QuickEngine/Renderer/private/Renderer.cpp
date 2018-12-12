
#include <Renderer.h>

#include "Renderer.h"

using namespace QE;


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
    context = _context;
    addRenderPass("GeometryPass", new GeometryPass(context, this));
    addRenderPass("DeferredLightingPass", new DeferredLightingPass(context));
    addRenderPass("SSAOPass", new SSAOPass(context));
    addRenderPass("PostPass", new PostPass(context));
}

Renderer::~Renderer() {
    int size = (int) pipelineList->size();
    for (int i = size - 1; i >= 0; i--) {
        QE::RendererImpl::RenderPassContainer *passC = (*pipelineList)[i];
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
    v_SSAOSize[0] = width * 1;
    v_SSAOSize[1] = height * 1;
    v_SSAOScale[0] = v_SSAOSize[0] / v_ScreenSize[0];
    v_SSAOScale[1] = v_SSAOSize[1] / v_ScreenSize[1];
    for (auto &i : *pipelineList) {
        i->renderPass->onResize(width, height);
    }
}

void Renderer::destroy() {

}

void Renderer::attachWorld(World *world) {
    this->world = world;
}

void Renderer::detachCurrentWorld() {
    world = nullptr;
}

void Renderer::doRender() {
    if (!world)
        return;

    for (auto &i : *pipelineList) {
        i->renderPass->doDraw();
    }
}

World *Renderer::getWorld() {
    return this->world;
}



