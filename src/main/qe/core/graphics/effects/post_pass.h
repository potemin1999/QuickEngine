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

namespace QEGraphics {

    struct PostPassInitData {
        float **v_PixelSize;
        float **v_ScreenSize;
        float **v_Vignette;
        float **v_PostBrightness;
        float **v_Gamma;
        int **v_FXAA;
        int **v_DrawingMode;
        Texture **t_AlbedoTex;
        Texture **t_PositionTex;
        Texture **t_NormalTex;
        Texture **t_OcclusionTex;
    };

    class PostPass : RenderPass {

    private:

        Shader *s_PostShader;
        float **v_PixelSize;
        float **v_ScreenSize;
        float **v_Vignette;
        float **v_PostBrightness;
        float **v_Gamma;
        int **v_FXAA;
        int **v_DrawingMode;
        Texture **t_AlbedoTex;
        Texture **t_PositionTex;
        Texture **t_NormalTex;
        Texture **t_OcclusionTex;

        unsigned vao;

        const float vert[18] =
                {-1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f};

    public:

        PostPass() : RenderPass() {

        }

        ~PostPass() {
            delete s_PostShader;
        }

        void init(PostPassInitData *initData) {
            v_PixelSize = initData->v_PixelSize;
            v_ScreenSize = initData->v_ScreenSize;
            v_Vignette = initData->v_Vignette;
            v_PostBrightness = initData->v_PostBrightness;
            v_Gamma = initData->v_Gamma;
            v_FXAA = initData->v_FXAA;
            v_DrawingMode = initData->v_DrawingMode;
            t_AlbedoTex = initData->t_AlbedoTex;
            t_PositionTex = initData->t_PositionTex;
            t_NormalTex = initData->t_NormalTex;
            t_OcclusionTex = initData->t_OcclusionTex;

            unsigned int post_vbo;
            glGenBuffers(1, &post_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
            glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            glBindVertexArray(0);
        }

        void compileShaders() {
            s_PostShader = new Shader("shaders/post_shader_vertex.glsl", "shaders/post_shader_fragment.glsl");
            s_PostShader->compile();
            s_PostShader->uniform("u_DrawingMode");
            s_PostShader->uniform("u_PixelSize");
            s_PostShader->uniform("u_ScreenSize");
            s_PostShader->uniform("u_SuperSampling");
            s_PostShader->uniform("u_Vignette");
            s_PostShader->uniform("u_FXAA");
            s_PostShader->uniform("u_Gamma");
            s_PostShader->uniform("u_Brightness");
            s_PostShader->uniform("u_AlbedoSpecTex");
            s_PostShader->uniform("u_NormalTex");
            s_PostShader->uniform("u_PositionTex");
            s_PostShader->uniform("u_DiffuseTex");
            s_PostShader->uniform("u_OcclusionTex");
        }

        void onResize(int width, int height) {

        }

        void doDraw() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            s_PostShader->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            s_PostShader->uniform1i("u_AlbedoSpecTex", 0);
            glActiveTexture(GL_TEXTURE0);
            (*t_AlbedoTex)->use();
            s_PostShader->uniform1i("u_NormalTex", 1);
            glActiveTexture(GL_TEXTURE1);
            (*t_NormalTex)->use();
            s_PostShader->uniform1i("u_PositionTex", 2);
            glActiveTexture(GL_TEXTURE2);
            (*t_PositionTex)->use();
            s_PostShader->uniform1i("u_DiffuseTex", 3);
            glActiveTexture(GL_TEXTURE3);
            (*t_AlbedoTex)->use();
            s_PostShader->uniform1i("u_OcclusionTex", 4);
            glActiveTexture(GL_TEXTURE4);
            (*t_OcclusionTex)->use();

            s_PostShader->uniform2f("u_PixelSize", (*v_PixelSize)[0], (*v_PixelSize)[1]);
            s_PostShader->uniform2f("u_ScreenSize", (*v_ScreenSize)[0], (*v_ScreenSize)[1]);
            s_PostShader->uniform1i("u_SuperSampling", 1);
            s_PostShader->uniform1f("u_Vignette", **v_Vignette);
            s_PostShader->uniform1f("u_Brightness", **v_PostBrightness);
            s_PostShader->uniform1i("u_FXAA", **v_FXAA);
            s_PostShader->uniform1f("u_Gamma", **v_Gamma);
            s_PostShader->uniform1i("u_DrawingMode", **v_DrawingMode);

            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }
    };
}
#endif //QUICKENGINE_POST_PASS_H
