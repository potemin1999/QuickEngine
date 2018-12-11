//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_RENDERER_H
#define QUICKENGINE_RENDERER_H

#include "engine_context.h"
#include "util/log.h"
#include "render_pass.h"
#include "effects/post_pass.h"
#include "geometry/geometry_pass.h"
#include "lighting/deferred_lighing_pass.h"
#include "lighting/ssao_pass.h"
#include "input/input_event_manager.h"
#include "input/input_receiver.h"
#include "world.h"
#include "util/list.h"
#include "util/strmap.h"
#include "util/render_data_storage.h"
#include <vector>

namespace QECore {

    namespace RendererImpl {
        class RendererInputReceiver;

        class RenderPassContainer;
    }

    class Renderer {
    private:

        RendererImpl::RendererInputReceiver *inputListener;
        RenderDataStorage *renderDataStorage;
        EngineContext *context;
        World *world;

        StrMap<int> *pipelineListNameMap;
        List<RendererImpl::RenderPassContainer *> *pipelineList;

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
