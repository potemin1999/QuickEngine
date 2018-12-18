//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_RENDERER_H
#define QUICKENGINE_RENDERER_H

#include "EngineContext.h"
#include "util/Log.h"
#include "RenderPass.h"
#include "effects/PostPass.h"
#include "geometry/GeometryPass.h"
#include "lighting/DeferredLightingPass.h"
#include "lighting/SSAOPass.h"
#include "input/InputEventManager.h"
#include "input/InputReceiver.h"
#include "World.h"
#include "util/RenderDataStorage.h"
#include <vector>

namespace QE {

    namespace RendererImpl {
        class RendererInputReceiver;

        class RenderPassContainer;
    }

    class Renderer {
    private:
        RenderDataStorage *renderDataStorage;
        EngineContext *context;
        World *world;

        std::map<std::string, int> pipelineListNameMap;
        std::vector<RendererImpl::RenderPassContainer *> pipelineList;

        /*RenderPass *p_GeometryPass;
        RenderPass *p_DeferredLightingPass;
        RenderPass *p_SSAOPass;
        RenderPass *p_PostPass;
        */

        float v_PixelSize[2];
        float v_ScreenSize[2];
        float v_SSAOScale[2];
        int v_SSAOSize[2];

        int v_SSAOSamples = 24;
        float v_Vignette = 1.0f;
    public:
        float v_Brightness = 1.0f;
    private:
        float v_PostBrightness = 1.0f;
        int v_DrawingMode = 0;
        int v_FXAA = 1;
        float v_Gamma = 1.0f;

    public:

        explicit Renderer(EngineContext *context);

        ~Renderer();

        void init(RenderDataStorage *storage);

        void addRenderPass(const char *passName, RenderPass *renderPass);

        void removeRenderPass(const char *passName);

        void compileShaders();

        void onResize(int width, int height);

        void attachWorld(World *world);

        void doRender();

        void detachCurrentWorld();

        void destroy();

    };

}
#endif //QUICKENGINE_RENDERER_H
