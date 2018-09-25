//
// Created by Ilya on 3/16/2018.
//

#ifndef QUICKENGINE_SSAO_PASS_H
#define QUICKENGINE_SSAO_PASS_H

#include "qe/core/graphics/defines.h"
#include "qe/core/graphics/basic/render_pass.h"
#include "qe/core/graphics/basic/shader.h"
#include "qe/core/graphics/basic/framebuffer.h"
#include "qe/core/graphics/basic/shader.h"
#include "qe/core/graphics/basic/texture.h"

namespace QEGraphics{

    struct SSAOPassInitData{
        Texture** t_Depthmap;
        Texture** t_NormalTex;
        Texture** t_PositionTex;
        Texture** t_LightingTex;
        Camera** camera;
        int** v_SSAOSamples;
        float** v_PixelSize;
        float** v_SSAOScale;
    };

    class SSAOPass : RenderPass{

    private:

        Framebuffer *f_SSAOFramebuffer = 0;
        Shader *s_SSAOShader = 0;
        Texture **t_Depthmap = 0;
        Texture **t_NormalTex = 0;
        Texture **t_PositionTex = 0;
        Texture **t_LightingTex = 0;
        Texture *t_OcclusionTex = 0;
        int **v_SSAOSamples = 0;
        float *v_SSAORandomMap = 0;
        float **v_PixelSize = 0;
        float **v_SSAOScale = 0;
        Camera **camera = 0;
        unsigned vao;
        unsigned *attachmentsConst = 0;

        const float vert[18] =
                { -1.0f,-1.0f, 0.0f,     -1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
                  1.0f, 1.0f, 0.0f,      1.0f,-1.0f, 0.0f,   -1.0f,-1.0f, 0.0f };

        float random(float min,float max) {
            //srand(time(0));
            float r = (rand()*1.0f)/(RAND_MAX*1.0f);
            return min + (max - min)*r;
        }

    public:

        SSAOPass() {
            attachmentsConst = new unsigned[1]{GL_COLOR_ATTACHMENT4};
        }

        ~SSAOPass() {
            delete s_SSAOShader;
            if (attachmentsConst)
                delete attachmentsConst;
            if (v_SSAORandomMap)
                delete v_SSAORandomMap;
            if (t_OcclusionTex)
                delete t_OcclusionTex;
        }

        Texture** getOcclusionTexture(){
            return &t_OcclusionTex;
        }

        virtual void init(SSAOPassInitData* initData){
            log("ssao pass init\n");
            t_Depthmap = initData->t_Depthmap;
            t_LightingTex = initData->t_LightingTex;
            t_NormalTex = initData->t_NormalTex;
            t_PositionTex = initData->t_PositionTex;
            camera = initData->camera;
            v_PixelSize = initData->v_PixelSize;
            v_SSAOScale = initData->v_SSAOScale;
            v_SSAOSamples = initData->v_SSAOSamples;
            t_OcclusionTex = new Texture(GL_R8,GL_RED);
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

        virtual void onResize(int width,int height){
            log("ssao pass resize\n");
            if (v_SSAORandomMap)
                delete v_SSAORandomMap;
            v_SSAORandomMap = new float[3 * **v_SSAOSamples];
            log("random map:\n");
            for (int i = 0; i<**v_SSAOSamples; i++) {
                float x = random(-1, 1);
                float y = random(-1, 1);
                float z = random(-1, 0);
                float n = Camera::invSqrt(x * x + y * y + z * z);// *((i + 1.0f) / *v_SSAOSamples);
                float m = ((i + 1.0f) / (**v_SSAOSamples));// * ((i + 1.0f) / (*v_SSAOSamples)) ;
                v_SSAORandomMap[3 * i + 0] = x*n*m;
                v_SSAORandomMap[3 * i + 1] = y*n*m;
                v_SSAORandomMap[3 * i + 2] = z*n*m-0.1f;
                log("( %f , %f , %f )\n", x*n*m, y*n*m, z*n*m);
            }
            if (f_SSAOFramebuffer) {
                f_SSAOFramebuffer->free();
                delete f_SSAOFramebuffer;
            }
            f_SSAOFramebuffer = new Framebuffer();
            // else
               // f_SSAOFramebuffer->use();
            t_OcclusionTex->setSize(width,height);
            t_OcclusionTex->setData(0,GL_FLOAT);
            t_OcclusionTex->setFilters(GL_NEAREST,GL_NEAREST);
            f_SSAOFramebuffer->attach(GL_COLOR_ATTACHMENT4,t_OcclusionTex);
            f_SSAOFramebuffer->commitAttachments(attachmentsConst, 1);

        }

        virtual void compileShaders(){
            s_SSAOShader = new Shader("shaders/ssao_vertex_shader.glsl","shaders/ssao_fragment_shader.glsl");
            s_SSAOShader->compile();
            s_SSAOShader->uniform("u_RandomMap");
            s_SSAOShader->uniform("u_Projection");
            s_SSAOShader->uniform("u_Aspect");
            s_SSAOShader->uniform("u_Fov");
            s_SSAOShader->uniform("u_ZFar");
            s_SSAOShader->uniform("u_Samples");
            s_SSAOShader->uniform("u_Scale");
            s_SSAOShader->uniform("u_PixelSize");
            s_SSAOShader->uniform("u_DepthTex");
            s_SSAOShader->uniform("u_NormalTex");
            s_SSAOShader->uniform("u_PositionTex");
            s_SSAOShader->uniform("u_LightingTex");
        }

        virtual void doDraw() {
            //glBindFramebuffer(GL_FRAMEBUFFER,0);
            f_SSAOFramebuffer->use();
            s_SSAOShader->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            s_SSAOShader->uniform1i("u_DepthTex",0);
            glActiveTexture(GL_TEXTURE0);
            (*t_Depthmap)->use();
            s_SSAOShader->uniform1i("u_NormalTex",1);
            glActiveTexture(GL_TEXTURE1);
            (*t_NormalTex)->use();
            s_SSAOShader->uniform1i("u_PositionTex",2);
            glActiveTexture(GL_TEXTURE2);
            (*t_PositionTex)->use();
            s_SSAOShader->uniform1i("u_LightingTex",3);
            glActiveTexture(GL_TEXTURE3);
            (*t_LightingTex)->use();
            glBindVertexArray(vao);
            glEnableVertexAttribArray(0);
            s_SSAOShader->uniformMatrix4fv("u_Projection",1,0,(*camera)->value_ptr((*camera)->mProjectionMatrix));
            s_SSAOShader->uniform1f("u_ZFar",(*camera)->farZ);
            s_SSAOShader->uniform1f("u_Aspect",(*camera)->aspect);
            s_SSAOShader->uniform1f("u_Fov",(*camera)->fovy);
            s_SSAOShader->uniform1i("u_Samples",**v_SSAOSamples);
            s_SSAOShader->uniform3fv("u_RandomMap", **v_SSAOSamples,v_SSAORandomMap);
            s_SSAOShader->uniform2f("u_PixelSize",(*v_PixelSize)[0], (*v_PixelSize)[1]);
            s_SSAOShader->uniform2f("u_Scale", (*v_SSAOScale)[0], (*v_SSAOScale)[1]);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
        }
    };
}
#endif //QUICKENGINE_SSAO_PASS_H
