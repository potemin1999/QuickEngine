//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_DEFERRED_LIGHING_PASS_H
#define QUICKENGINE_DEFERRED_LIGHING_PASS_H

#include "render_pass.h"
#include "shader.h"
#include "framebuffer.h"
#include "shader.h"
#include "texture.h"

namespace QECore {

    class DeferredLightingPass : public RenderPass {

    private:
        Shader *s_LightShader = nullptr;
        Framebuffer *f_DefLightFramebuffer = nullptr;
        Texture *t_DefLightTex = nullptr;
        Texture *t_AlbedoTex = nullptr;
        Texture *t_PositionTex = nullptr;
        Texture *t_NormalTex = nullptr;
        float *v_PixelSize = nullptr;
        unsigned *attachmentsConst = nullptr;
        unsigned vao = 0;
        const float vert[18] =
                {-1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

    public:

        explicit DeferredLightingPass(EngineContext *context);

        ~DeferredLightingPass() override;

        void init(RenderDataStorage *storage) override;

        void compileShaders() override;

        void onResize(int width, int height) override;

        void doDraw() override;

    };

}
#endif //QUICKENGINE_DEFERRED_LIGHING_PASS_H
