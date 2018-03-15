//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_GEOMETRYPASS_H
#define QUICKENGINE_GEOMETRYPASS_H

#include "graphics/core/object.h"
#include "graphics/core/camera.h"
#include "graphics/defines.h"
#include "graphics/core/basic/framebuffer.h"
#include "graphics/core/basic/shader.h"
#include "graphics/core/basic/render_pass.h"

namespace QEGraphics{

    struct GeometryPassInitData{
        vector<Object*> *objects;
        Camera **camera;
        float** v_Brightness;
    };

    class GeometryPass : RenderPass{

    private:
        Framebuffer* f_MainFramebuffer;
        Texture* t_Depthmap;
        Texture* t_Albedo;
        Texture* t_Normal;
        Texture* t_Position;
        Shader *s_MainShader;
        vector<Object*> *objects;
        Camera *camera;
        float* v_Brightness;


    public:

        GeometryPass() : RenderPass(){}

        Texture** getAlbedoTex(){
            return &t_Albedo;
        }

        Texture** getNormalTex(){
            return &t_Normal;
        }

        Texture** getPositionTex(){
            return &t_Position;
        }

        Texture** getDepthmap(){
            return &t_Depthmap;
        }

        virtual void init(GeometryPassInitData* initData){
            log("geometry pass init\n");
            camera = *(initData->camera);
            objects = initData->objects;
            v_Brightness = *(initData->v_Brightness);
            t_Depthmap = new Texture(GL_DEPTH_COMPONENT24,GL_DEPTH_COMPONENT);
            t_Albedo = new Texture(GL_RGBA8,GL_RGBA);
            t_Normal = new Texture(GL_RGBA8,GL_RGBA);
            t_Position = new Texture(GL_RGBA8,GL_RGBA);
            //f_MainFramebuffer = new Framebuffer();
        }

        virtual void compileShaders(){
            s_MainShader = new Shader("shaders/shader_vertex.glsl","shaders/shader_fragment.glsl");
            s_MainShader->compile();
            s_MainShader->uniform("u_V");
            s_MainShader->uniform("u_P");
            s_MainShader->uniform("u_M");
            s_MainShader->uniform("u_MVP");
            s_MainShader->uniform("u_ZFar");
            s_MainShader->uniform("u_AmbientTex");
            s_MainShader->uniform("u_NormalTex");
            s_MainShader->uniform("u_Brightness");
        }

        virtual void onResize(int width,int height){
            log("geometry pass resize\n");
            //if (!f_MainFramebuffer)
                f_MainFramebuffer = new Framebuffer();
           // else
                //f_MainFramebuffer->use();
            t_Depthmap->setSize(width,height);
            t_Albedo->setSize(width,height);
            t_Position->setSize(width,height);
            t_Normal->setSize(width,height);
            t_Depthmap->setData(0,GL_FLOAT);
            t_Albedo->setData(0,GL_FLOAT);
            t_Position->setData(0,GL_FLOAT);
            t_Normal->setData(0,GL_FLOAT);
            t_Depthmap->setFilters(GL_NEAREST,GL_NEAREST);
            t_Albedo->setFilters(GL_NEAREST,GL_NEAREST);
            t_Normal->setFilters(GL_NEAREST,GL_NEAREST);
            t_Position->setFilters(GL_NEAREST,GL_NEAREST);
            f_MainFramebuffer->attachDepthmap(t_Depthmap);
            f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT0,t_Albedo);
            f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT1,t_Normal);
            f_MainFramebuffer->attach(GL_COLOR_ATTACHMENT2,t_Position);
            f_MainFramebuffer->commitAttachments(new unsigned[3]{GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2},3);
        }

        virtual void draw(){
            //glBindFramebuffer(GL_FRAMEBUFFER,0);
            f_MainFramebuffer->use();
            s_MainShader->use();
            glEnable(GL_DEPTH_TEST);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            s_MainShader->uniform1f("u_ZFar",camera->farZ);
            s_MainShader->uniform1f("u_Brightness",*v_Brightness);
            for (int j=0;j<(*objects).size();j++) {
                Object* o = (*objects)[j];
                for (int i = 0; i < o->mesh_count; i++) {
                    try {
                        Mesh* m = (o->meshes + i);
                        s_MainShader->uniformMatrix4fv("u_MVP",1,0,camera->value_ptr(camera->mVPMatrix*o->mModelMatrix));
                        s_MainShader->uniformMatrix4fv("u_V",1,0,camera->value_ptr(camera->mVMatrix));
                        s_MainShader->uniformMatrix4fv("u_P",1,0,camera->value_ptr(camera->mProjectionMatrix));
                        s_MainShader->uniformMatrix4fv("u_M",1,0,camera->value_ptr(o->mModelMatrix));
                        if (true) {
                            s_MainShader->uniform1i("u_AmbientTex",0);
                            glActiveTexture(GL_TEXTURE0);
                            if ( m->material->t_Ambient == nullptr) {
                                //printf("ERROR: mesh object %s has null ambient texture\n", m->name);
                                continue;
                            }else {
                                glBindTexture(GL_TEXTURE_2D, m->material->t_Ambient->id);
                                //printf(" ambient texture binded on link %i\n", m->material->t_Ambient->id);
                            }
                            s_MainShader->uniform1i("u_NormalTex",1);
                            //glUniform1i(*u_MainNormalTex, 1);
                            glActiveTexture(GL_TEXTURE1);
                            if (m->material->t_Normal != nullptr) {
                                glBindTexture(GL_TEXTURE_2D, m->material->t_Normal->id);
                                //printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
                            }else {
                                glBindTexture(GL_TEXTURE_2D, m->material->t_Ambient->id);
                                //printf(" normal texture binded on link %i\n", m->material->t_Ambient->id);
                            }
                        }
                        glBindVertexArray(m->data);
                        glEnableVertexAttribArray(0);
                        glEnableVertexAttribArray(1);
                        glEnableVertexAttribArray(2);
                        glDrawArrays(GL_TRIANGLES, 0, m->vert_count);
                        glDisableVertexAttribArray(0);
                        glDisableVertexAttribArray(1);
                        glDisableVertexAttribArray(2);
                        glBindVertexArray(0);
                    }
                    catch (...) {
                    }
                }
            }
        }

    };


}
#endif //QUICKENGINE_GEOMETRYPASS_H
