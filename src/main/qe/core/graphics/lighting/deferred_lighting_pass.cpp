//
// Created by ilya on 9/26/18.
//

#include <qe/core/graphics/lighting/deferred_lighing_pass.h>
#include <glad/glad.h>

using namespace QECore;

DeferredLightingPass::DeferredLightingPass(EngineContext *context) : RenderPass(context) {
    attachmentsConst = new unsigned[1]{GL_COLOR_ATTACHMENT4};
}

DeferredLightingPass::~DeferredLightingPass() {
    delete s_LightShader;
    delete attachmentsConst;
    delete t_DefLightTex;
}

void DeferredLightingPass::init(RenderDataStorage *storage) {
    v_PixelSize = (float*) storage->get("v_PixelSize");
    t_AlbedoTex = (Texture*) storage->get("t_Albedo");
    t_NormalTex = (Texture*) storage->get("t_Normal");
    t_PositionTex = (Texture*) storage->get("t_Position");
    unsigned int post_vbo;
    glGenBuffers(1, &post_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), vert, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    t_DefLightTex = new Texture(GL_RGBA8, GL_RGBA);
    storage->set("t_DefLighting",t_DefLightTex);
}

void DeferredLightingPass::compileShaders()  {
    s_LightShader = new Shader("shaders/light_vertex_shader.glsl", "shaders/light_fragment_shader.glsl");
    s_LightShader->compile();
    s_LightShader->uniform("u_PixelSize");
    s_LightShader->uniform("u_AlbedoSpecTex");
    s_LightShader->uniform("u_NormalTex");
    s_LightShader->uniform("u_PositionTex");
}

void DeferredLightingPass::onResize(int width, int height)  {
    if (f_DefLightFramebuffer) {
        f_DefLightFramebuffer->free();
        delete f_DefLightFramebuffer;
    }
    f_DefLightFramebuffer = new Framebuffer();
    t_DefLightTex->setSize(width, height);
    t_DefLightTex->setData(nullptr, GL_FLOAT);
    t_DefLightTex->setFilters(GL_NEAREST, GL_NEAREST);
    f_DefLightFramebuffer->attach(GL_COLOR_ATTACHMENT3, t_DefLightTex);
    f_DefLightFramebuffer->commitAttachments(attachmentsConst, 1);
}

void DeferredLightingPass::doDraw() {
    f_DefLightFramebuffer->use();
    s_LightShader->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    s_LightShader->uniform1i("u_AlbedoSpecTex", 0);
    glActiveTexture(GL_TEXTURE0);
    t_AlbedoTex->use();
    s_LightShader->uniform1i("u_NormalTex", 1);
    glActiveTexture(GL_TEXTURE1);
    t_NormalTex->use();
    s_LightShader->uniform1i("u_PositionTex", 2);
    glActiveTexture(GL_TEXTURE2);
    t_PositionTex->use();

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    s_LightShader->uniform2f("u_PixelSize", v_PixelSize[0], v_PixelSize[1]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}


