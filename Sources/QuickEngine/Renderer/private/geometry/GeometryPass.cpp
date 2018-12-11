//
// Created by ilya on 9/26/18.
//

#include <geometry/GeometryPass.h>
#include <util/Log.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <Texture.h>
#include <GameObject.h>


GeometryPass::GeometryPass(EngineContext *context) : RenderPass(context) {
    attachmentConst = new unsigned[3]{GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
}

GeometryPass::~GeometryPass() {
    delete s_MainShader;
    delete attachmentConst;
}

void GeometryPass::init(RenderDataStorage *storage) {
    log("geometry pass new init\n");
    t_Depthmap = new Texture(GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT);
    t_Albedo = new Texture(GL_RGBA8, GL_RGBA);
    t_Normal = new Texture(GL_RGBA8, GL_RGBA);
    t_Position = new Texture(GL_RGBA8, GL_RGBA);
    camera = (Camera *) storage->require("с_CurrentWorldCamera");
    objects = (vector<GameObject *> *) storage->require("objects");
    v_Brightness = (float *) storage->require("v_Brightness");
    storage->set("t_Depthmap", t_Depthmap);
    storage->set("t_Albedo", t_Albedo);
    storage->set("t_Normal", t_Normal);
    storage->set("t_Position", t_Position);
}

void GeometryPass::onResize(int width, int height) {
    if (f_MainFramebuffer) {
        f_MainFramebuffer->free();
        delete f_MainFramebuffer;
    }
    f_MainFramebuffer = new Framebuffer();
    t_Depthmap->setSize(width, height);
    t_Albedo->setSize(width, height);
    t_Position->setSize(width, height);
    t_Normal->setSize(width, height);
    t_Depthmap->setData(nullptr, GL_FLOAT);
    t_Albedo->setData(nullptr, GL_FLOAT);
    t_Position->setData(nullptr, GL_FLOAT);
    t_Normal->setData(nullptr, GL_FLOAT);
    t_Depthmap->setFilters(GL_NEAREST, GL_NEAREST);
    t_Albedo->setFilters(GL_NEAREST, GL_NEAREST);
    t_Normal->setFilters(GL_NEAREST, GL_NEAREST);
    t_Position->setFilters(GL_NEAREST, GL_NEAREST);
    f_MainFramebuffer->attachDepthmap(t_Depthmap);
    f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT0, t_Albedo);
    f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT1, t_Normal);
    f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT2, t_Position);
    f_MainFramebuffer->commitAttachments(attachmentConst, 3);
}

void GeometryPass::compileShaders() {
    s_MainShader = new Shader("shaders/shader_vertex.glsl", "shaders/shader_fragment.glsl");
    s_MainShader->compile();
    s_MainShader->uniform("u_View");
    s_MainShader->uniform("u_CamPos");
    s_MainShader->uniform("u_MVP");
    s_MainShader->uniform("u_ZFar");
    s_MainShader->uniform("u_AmbientTex");
    s_MainShader->uniform("u_NormalTex");
    s_MainShader->uniform("u_Brightness");
}

void GeometryPass::doDraw() {
    //glBindFramebuffer(GL_FRAMEBUFFER,0);
    f_MainFramebuffer->use();
    s_MainShader->use();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    s_MainShader->uniform1f("u_ZFar", camera->getFarZ());
    s_MainShader->uniform1f("u_Brightness", *v_Brightness);
    s_MainShader->uniform3fv("u_CamPos", 1, camera->getPosition());
    s_MainShader->uniformMatrix4fv("u_View", 1, false, camera->getViewMatrixPtr());
    glm::mat4 mVP = *camera->getVPMatrix();

    // TODO: rewrite this shit
    for (auto o : *objects) {
        for (int i = 0; i < o->mesh_count; i++) {
            Mesh *m = (o->meshes + i);
            glm::mat4 mM;

            if (o->rigidBody)
                mM = o->getModelMatrix();
            else {
                // TODO: reformat this and add rotation support
                mM = glm::mat4(1);
                mM = glm::translate(mM, o->getPos());
            }

            glm::mat4 mat = mVP * mM;
            s_MainShader->uniformMatrix4fv("u_MVP", 1, false, glm::value_ptr(mat));

            s_MainShader->uniform1i("u_AmbientTex", 0);
            glActiveTexture(GL_TEXTURE0);
            if (m->material->t_Ambient == nullptr) {
//                printf("ERROR: mesh object %s has null ambient texture\n", m->name);
                continue;
            } else {
                glBindTexture(GL_TEXTURE_2D, m->material->t_Ambient->id);
                //printf(" ambient texture binded on link %i\n", m->material->t_Ambient->id);
            }
            s_MainShader->uniform1i("u_NormalTex", 1);
            //glUniform1i(*u_MainNormalTex, 1);
            glActiveTexture(GL_TEXTURE1);
            if (m->material->t_Normal != nullptr) {
                glBindTexture(GL_TEXTURE_2D, m->material->t_Normal->id);
                //printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
            } else {
                continue;
                //printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
            }

            glBindVertexArray((GLuint) m->data);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glDrawArrays(GL_TRIANGLES, 0, m->vert_count);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glBindVertexArray(0);
        }
    }
}
