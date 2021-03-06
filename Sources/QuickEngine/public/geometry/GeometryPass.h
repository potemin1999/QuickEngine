//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_GEOMETRYPASS_H
#define QUICKENGINE_GEOMETRYPASS_H

#include "GameObject.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "RenderPass.h"

namespace QE {

    class GeometryPass : public RenderPass {
    private:

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

        explicit GeometryPass(EngineContext *context);

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
#endif //QUICKENGINE_GEOMETRYPASS_H
