#pragma once

#include "RenderPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "input/InputKey.h"
#include "input/InputProcessingResult.h"

namespace QE {

    class PostPass : public RenderPass {

    private:

        class PostPassInputReceiver;

        EngineContext *engineContext;
        InputReceiver *inputReceiver;
        Shader *s_PostShader;
        float *v_PixelSize;
        float *v_ScreenSize;
        float *v_Vignette;
        float *v_PostBrightness;
        float *v_Gamma;
        float *v_SSAOScale;
        int *v_FXAA;
        int *v_DrawingMode;
        Texture *t_AlbedoTex;
        Texture *t_PositionTex;
        Texture *t_NormalTex;
        Texture *t_OcclusionTex;

        unsigned vao;

        const float vert[18] =
                {-1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f,
                 -1.0f, 1.0f, 0.0f};

    public:

        explicit PostPass(EngineContext *context);

        ~PostPass() override;

        void init(RenderDataStorage *storage) override;

        void compileShaders() override;

        void onResize(int width, int height) override;

        void doDraw() override;

    private:

        class PostPassInputReceiver : InputReceiver {
        private:

            PostPass *parent;

        public:

            explicit PostPassInputReceiver(PostPass *_parent) : InputReceiver() {
                parent = _parent;
            }

            virtual InputProcessResult onInputEvent(InputEvent *event);

        };

    };

}