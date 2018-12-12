#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "RenderPass.h"
#include "Renderer.h"

namespace QE {
    class Renderer;

    class GeometryPass : public RenderPass {
    private:

        Renderer *renderer;
        Framebuffer *f_MainFramebuffer = nullptr;
        Texture *t_Depthmap = nullptr;
        Texture *t_Albedo = nullptr;
        Texture *t_Normal = nullptr;
        Texture *t_Position = nullptr;
        Shader *s_MainShader = nullptr;
        Camera *camera = nullptr;
        float *v_Brightness = nullptr;
        unsigned *attachmentConst = nullptr;

    public:

        explicit GeometryPass(EngineContext *context, Renderer *renderer);

        ~GeometryPass() override;

        Texture **getAlbedoTex() {
            return &t_Albedo;
        }

        Texture **getNormalTex() {
            return &t_Normal;
        }

        Texture **getPositionTex() {
            return &t_Position;
        }

        Texture **getDepthmap() {
            return &t_Depthmap;
        }

        void init(RenderDataStorage *storage) override;

        void compileShaders() override;

        void onResize(int width, int height) override;

        void doDraw() override;

    };


}