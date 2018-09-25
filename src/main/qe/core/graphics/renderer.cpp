//
// Created by ilya on 9/24/18.
//

#include <qe/core/graphics/renderer.h>

using namespace QEGraphics;

Renderer::Renderer(EngineContext *_context) {
    inputListener = new RendererInputReceiver(this);
    context = _context;
    context->getInputEventManager()->registerInputReceiver(inputListener);
}

Renderer::~Renderer() {
    context->getInputEventManager()->unregisterInputReceiver(inputListener);
    delete inputListener;
}

void Renderer::prepare() {
    p_GeometryPass = (RenderPass *) new GeometryPass();
    p_DeferredLightingPass = (RenderPass *) new DeferredLightingPass();
    p_SSAOPass = (RenderPass *) new SSAOPass();
    p_PostPass = (RenderPass *) new PostPass();
}

void Renderer::destroy() {
    delete p_PostPass;
    delete p_SSAOPass;
    delete p_DeferredLightingPass;
    delete p_GeometryPass;
}

void Renderer::attachWorld(World *world) {

}

void Renderer::detachCurrentWorld() {

}

void Renderer::doRender() {

}



