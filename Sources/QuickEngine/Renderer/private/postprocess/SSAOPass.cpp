//
// Created by ilya on 9/26/18.
//

#include "lighting/SSAOPass.h"
#include "util/Log.h"
#include "util/Utils.h"
#include "OpenGL.h"

void SSAOPass::init(RenderDataStorage *storage) {
    log("ssao pass new init\n");
    camera = (Camera *) storage->require("с_CurrentWorldCamera");
    v_PixelSize = (float *) storage->require("v_PixelSize");
    v_SSAOScale = (float *) storage->require("v_SSAOScale");
    v_SSAOSamples = (int *) storage->require("v_SSAOSamples");
    t_Depthmap = (Texture *) storage->require("t_Depthmap");
    t_LightingTex = (Texture *) storage->require("t_DefLighting");
    t_NormalTex = (Texture *) storage->require("t_Normal");
    t_PositionTex = (Texture *) storage->require("t_Position");

    t_OcclusionTex = new Texture(GL_R8, GL_RED);
    unsigned int post_vbo;
    glGenBuffers(1, &post_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), vert, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    storage->set("t_Occlusion", t_OcclusionTex);
}

SSAOPass::SSAOPass(EngineContext *context) : RenderPass(context) {
    attachmentsConst = new unsigned[1]{GL_COLOR_ATTACHMENT4};
}

SSAOPass::~SSAOPass() {
    delete s_SSAOShader;
    if (attachmentsConst)
        delete attachmentsConst;
    if (v_SSAORandomMap)
        delete v_SSAORandomMap;
    if (t_OcclusionTex)
        delete t_OcclusionTex;
}

void SSAOPass::onResize(int width, int height) {
    log("ssao pass resize\n");
    if (v_SSAORandomMap)
        delete v_SSAORandomMap;
    v_SSAORandomMap = new float[3 * *v_SSAOSamples];

    log("random map:\n");
    for (int i = 0; i < *v_SSAOSamples; i++) {
        float x = QE::random(-1, 1);
        float y = QE::random(-1, 1);
        float z = QE::random(-1, 0);
        float n = Camera::invSqrt(x * x + y * y + z * z);// *((i + 1.0f) / *v_SSAOSamples);
        float m = ((i + 1.0f) / (*v_SSAOSamples));// * ((i + 1.0f) / (*v_SSAOSamples)) ;
        v_SSAORandomMap[3 * i + 0] = x * n * m;
        v_SSAORandomMap[3 * i + 1] = y * n * m;
        v_SSAORandomMap[3 * i + 2] = z * n * m - 0.0f;
        log("( %f , %f , %f )\n", x * n * m, y * n * m, z * n * m);
    }
    if (f_SSAOFramebuffer) {
        f_SSAOFramebuffer->free();
        delete f_SSAOFramebuffer;
    }
    f_SSAOFramebuffer = new Framebuffer();
    // else
    // f_SSAOFramebuffer->use();
    t_OcclusionTex->setSize(width * v_SSAOScale[0], height * v_SSAOScale[1]);
    t_OcclusionTex->setData(0, GL_FLOAT);
    t_OcclusionTex->setFilters(GL_NEAREST, GL_NEAREST);
    f_SSAOFramebuffer->attach(GL_COLOR_ATTACHMENT4, t_OcclusionTex);
    f_SSAOFramebuffer->commitAttachments(attachmentsConst, 1);
    log("ssao pass resize done\n");
}

void SSAOPass::compileShaders() {
    s_SSAOShader = new Shader("shaders/ssao_vertex_shader.glsl", "shaders/ssao_fragment_shader.glsl");
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

void SSAOPass::doDraw() {
    //glBindFramebuffer(GL_FRAMEBUFFER,0);
    f_SSAOFramebuffer->use();
    s_SSAOShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    s_SSAOShader->uniform1i("u_DepthTex", 0);
    glActiveTexture(GL_TEXTURE0);
    t_Depthmap->use();
    s_SSAOShader->uniform1i("u_NormalTex", 1);
    glActiveTexture(GL_TEXTURE1);
    t_NormalTex->use();
    s_SSAOShader->uniform1i("u_PositionTex", 2);
    glActiveTexture(GL_TEXTURE2);
    t_PositionTex->use();
    s_SSAOShader->uniform1i("u_LightingTex", 3);
    glActiveTexture(GL_TEXTURE3);
    t_LightingTex->use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    s_SSAOShader->uniformMatrix4fv("u_Projection", 1, 0, camera->getProjectionMatrixPtr());
    s_SSAOShader->uniform1f("u_ZFar", camera->getFarZ());
    s_SSAOShader->uniform1f("u_Aspect", camera->getAspect());
    s_SSAOShader->uniform1f("u_Fov", camera->getFov());
    s_SSAOShader->uniform1i("u_Samples", *v_SSAOSamples);
    s_SSAOShader->uniform3fv("u_RandomMap", *v_SSAOSamples, v_SSAORandomMap);
    s_SSAOShader->uniform2f("u_PixelSize", v_PixelSize[0], v_PixelSize[1]);
    s_SSAOShader->uniform2f("u_Scale", v_SSAOScale[0], v_SSAOScale[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}
