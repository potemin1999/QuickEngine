//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_SSAO_PASS_H
#define QUICKENGINE_SSAO_PASS_H

#include "Camera.h"
#include "Defines.h"
#include "RenderPass.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"

namespace QE {

    class SSAOPass : public RenderPass {

    private:

        Framebuffer *f_SSAOFramebuffer = nullptr;
        Shader *s_SSAOShader = nullptr;
        Texture *t_Depthmap = nullptr;
        Texture *t_NormalTex = nullptr;
        Texture *t_PositionTex = nullptr;
        Texture *t_LightingTex = nullptr;
        Texture *t_OcclusionTex = nullptr;
        int *v_SSAOSamples = nullptr;
        float *v_SSAORandomMap = nullptr;
        float *v_PixelSize = nullptr;
        float *v_SSAOScale = nullptr;
        Camera *camera = nullptr;
        unsigned vao;
        unsigned *attachmentsConst = nullptr;

        const float vert[18] =
                {-1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f,
                 -1.0f, 1.0f, 0.0f,};

    public:

        explicit SSAOPass(EngineContext *context);

        ~SSAOPass() override;

        void init(RenderDataStorage *storage) override;

        void onResize(int width, int height) override;

        void compileShaders() override;

        void doDraw() override;

    };
}
#endif //QUICKENGINE_SSAO_PASS_H
