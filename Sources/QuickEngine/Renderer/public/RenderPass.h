#pragma once

#include "util/RenderDataStorage.h"
#include "EngineContext.h"
#include "World.h"
#include "Camera.h"

namespace QE {

    class RenderPass {

    public:

        explicit RenderPass(EngineContext *context) {};

        virtual ~RenderPass() = default;

        virtual void init(RenderDataStorage *storage) {};

        virtual void compileShaders() {};

        virtual void onWorldAttached(World *world) {};

        virtual void onResize(int width, int height) {};

        virtual void onWorldDetached(World *world) {};

        virtual void onCameraUpdated(Camera *camera) {};

        virtual void doDraw() {};

    };
}
