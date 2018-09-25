//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_RENDERER_H
#define QUICKENGINE_RENDERER_H

#include <qe/core/engine_context.h>
#include <qe/core/util/log.h>
#include <qe/core/graphics/basic/render_pass.h>
#include <qe/core/graphics/effects/post_pass.h>
#include <qe/core/graphics/geometry/geometry_pass.h>
#include <qe/core/graphics/lighting/deferred_lighing_pass.h>
#include <qe/core/graphics/lighting/ssao_pass.h>
#include <qe/core/input/input_event_manager.h>
#include <qe/core/input/input_receiver.h>
#include <qe/core/world/world.h>


using namespace QECore;
using namespace QEInput;

namespace QEGraphics {

    class RendererInputReceiver;

    class Renderer {
    private:
        RendererInputReceiver *inputListener;
        EngineContext *context;
        RenderPass *p_GeometryPass;
        RenderPass *p_DeferredLightingPass;
        RenderPass *p_SSAOPass;
        RenderPass *p_PostPass;

    public:
        Renderer(EngineContext *context);

        ~Renderer();

        void prepare();

        void attachWorld(World *world);

        void doRender();

        void detachCurrentWorld();

        void destroy();
    };

    class RendererInputReceiver : public InputReceiver {
    private:
        Renderer *parent;
    public:
        RendererInputReceiver(Renderer *renderer);

        InputProcessResult onInputEvent(InputEvent *event) override;
    };

}
#endif //QUICKENGINE_RENDERER_H
