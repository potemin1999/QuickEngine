//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_DEFERRED_LIGHING_PASS_H
#define QUICKENGINE_DEFERRED_LIGHING_PASS_H

#include "qe/core/graphics/defines.h"
#include "qe/core/graphics/basic/render_pass.h"
#include "qe/core/graphics/basic/shader.h"
#include "qe/core/graphics/basic/framebuffer.h"
#include "qe/core/graphics/basic/shader.h"
#include "qe/core/graphics/basic/texture.h"

namespace QEGraphics {

    struct DeferredLightingPassInitData {
        Texture **t_AlbedoTex;
        Texture **t_PositionTex;
        Texture **t_NormalTex;
        float **v_PixelSize;
    };

    class DeferredLightingPass : RenderPass {

    private:
        Shader *s_LightShader = 0;
        Framebuffer *f_DefLightFramebuffer = 0;
        Texture *t_DefLightTex = 0;
        Texture **t_AlbedoTex = 0;
        Texture **t_PositionTex = 0;
        Texture **t_NormalTex = 0;
        float **v_PixelSize = 0;
        unsigned *attachmentsConst = 0;
        unsigned vao = 0;
        const float vert[18] =
                {-1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

    public:

        DeferredLightingPass() {
            attachmentsConst = new unsigned[1]{GL_COLOR_ATTACHMENT4};
        }

        ~DeferredLightingPass() {
            delete s_LightShader;
            if (attachmentsConst)
                delete attachmentsConst;
            if (t_DefLightTex)
                delete t_DefLightTex;
        }

        void init(DeferredLightingPassInitData *initData) {
            v_PixelSize = initData->v_PixelSize;
            t_AlbedoTex = initData->t_AlbedoTex;
            t_NormalTex = initData->t_NormalTex;
            t_PositionTex = initData->t_PositionTex;
            unsigned int post_vbo;
            glGenBuffers(1, &post_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
            glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
            t_DefLightTex = new Texture(GL_RGBA8, GL_RGBA);
        }

        void compileShaders() {
            s_LightShader = new Shader("shaders/light_vertex_shader.glsl", "shaders/light_fragment_shader.glsl");
            s_LightShader->compile();
            s_LightShader->uniform("u_PixelSize");
            s_LightShader->uniform("u_AlbedoSpecTex");
            s_LightShader->uniform("u_NormalTex");
            s_LightShader->uniform("u_PositionTex");
        }

        void onResize(int width, int height) {
            if (f_DefLightFramebuffer) {
                f_DefLightFramebuffer->free();
                delete f_DefLightFramebuffer;
            }
            f_DefLightFramebuffer = new Framebuffer();
            t_DefLightTex->setSize(width, height);
            t_DefLightTex->setData(0, GL_FLOAT);
            t_DefLightTex->setFilters(GL_NEAREST, GL_NEAREST);
            f_DefLightFramebuffer->attach(GL_COLOR_ATTACHMENT3, t_DefLightTex);
            f_DefLightFramebuffer->commitAttachments(attachmentsConst, 1);
        }

        void doDraw() {
            f_DefLightFramebuffer->use();
            s_LightShader->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            s_LightShader->uniform1i("u_AlbedoSpecTex", 0);
            glActiveTexture(GL_TEXTURE0);
            (*t_AlbedoTex)->use();
            s_LightShader->uniform1i("u_NormalTex", 1);
            glActiveTexture(GL_TEXTURE1);
            (*t_NormalTex)->use();
            s_LightShader->uniform1i("u_PositionTex", 2);
            glActiveTexture(GL_TEXTURE2);
            (*t_PositionTex)->use();

            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            s_LightShader->uniform2f("u_PixelSize", (*v_PixelSize)[0], (*v_PixelSize)[1]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }
    };

}
#endif //QUICKENGINE_DEFERRED_LIGHING_PASS_H
