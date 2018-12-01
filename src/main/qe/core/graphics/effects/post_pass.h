//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_POST_PASS_H
#define QUICKENGINE_POST_PASS_H

#include <qe/core/graphics/defines.h>
#include <qe/core/graphics/basic/render_pass.h>
#include <qe/core/graphics/basic/framebuffer.h>
#include <qe/core/graphics/basic/shader.h>
#include <qe/core/graphics/basic/texture.h>
#include <qe/core/input/input_key.h>
#include <qe/core/input/input_processing_result.h>

using namespace QECore;

namespace QECore {

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
                 -1.0f, 1.0f, 0.0f,};
        //{-1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
          //          1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

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

            PostPassInputReceiver(PostPass *_parent) : InputReceiver() {
                parent = _parent;
            }

            virtual InputProcessResult onInputEvent(InputEvent *event);

        };

    };

}
#endif //QUICKENGINE_POST_PASS_H
