//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_RENDER_PASS_H
#define QUICKENGINE_RENDER_PASS_H

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
#endif //QUICKENGINE_RENDER_PASS_H
