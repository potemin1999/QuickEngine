//
// Created by ilya on 9/26/18.
//

#include <qe/core/graphics/effects/post_pass.h>

using namespace QECore;

PostPass::PostPass(EngineContext *context) : RenderPass(context) {
    engineContext = context;
    inputReceiver = (InputReceiver *) new PostPassInputReceiver(this);
    engineContext->getInputEventManager()->registerInputReceiver(inputReceiver);
}

PostPass::~PostPass() {
    engineContext->getInputEventManager()->unregisterInputReceiver(inputReceiver);
    delete inputReceiver;
    delete s_PostShader;
}

void PostPass::init(RenderDataStorage* storage){
    v_PixelSize = (float*) storage->require("v_PixelSize");
    v_ScreenSize = (float*) storage->require("v_ScreenSize");
    v_Vignette = (float*) storage->require("v_Vignette");
    v_PostBrightness = (float*) storage->require("v_PostBrightness");
    v_Gamma = (float*) storage->require("v_Gamma");
    v_FXAA = (int*) storage->require("v_FXAA");
    v_SSAOScale = (float*) storage->require("v_SSAOScale");
    v_DrawingMode = (int*) storage->require("v_DrawingMode");
    t_AlbedoTex = (Texture*) storage->require("t_Albedo");
    t_NormalTex = (Texture*) storage->require("t_Normal");
    t_PositionTex = (Texture*) storage->require("t_Position");
    t_OcclusionTex = (Texture*) storage->require("t_Occlusion");

    unsigned int post_vbo;
    glGenBuffers(1, &post_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, post_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(float), vert, GL_STATIC_DRAW);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void PostPass::compileShaders(){
    s_PostShader = new Shader("shaders/post_shader_vertex.glsl", "shaders/post_shader_fragment.glsl");
    s_PostShader->compile();
    s_PostShader->uniform("u_DrawingMode");
    s_PostShader->uniform("u_PixelSize");
    s_PostShader->uniform("u_ScreenSize");
    s_PostShader->uniform("u_SuperSampling");
    s_PostShader->uniform("u_Vignette");
    s_PostShader->uniform("u_FXAA");
    s_PostShader->uniform("u_Gamma");
    s_PostShader->uniform("u_SSAOScale");
    s_PostShader->uniform("u_Brightness");
    s_PostShader->uniform("u_AlbedoSpecTex");
    s_PostShader->uniform("u_NormalTex");
    s_PostShader->uniform("u_PositionTex");
    s_PostShader->uniform("u_DiffuseTex");
    s_PostShader->uniform("u_OcclusionTex");
}

void PostPass::doDraw(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    s_PostShader->use();
    glClear( ((unsigned)GL_COLOR_BUFFER_BIT) | ((unsigned)GL_DEPTH_BUFFER_BIT) );
    s_PostShader->uniform1i("u_AlbedoSpecTex", 0);
    glActiveTexture(GL_TEXTURE0);
    t_AlbedoTex->use();
    s_PostShader->uniform1i("u_NormalTex", 1);
    glActiveTexture(GL_TEXTURE1);
    t_NormalTex->use();
    s_PostShader->uniform1i("u_PositionTex", 2);
    glActiveTexture(GL_TEXTURE2);
    t_PositionTex->use();
    s_PostShader->uniform1i("u_DiffuseTex", 3);
    glActiveTexture(GL_TEXTURE3);
    t_AlbedoTex->use();
    s_PostShader->uniform1i("u_OcclusionTex", 4);
    glActiveTexture(GL_TEXTURE4);
    t_OcclusionTex->use();
    s_PostShader->uniform2f("u_PixelSize", v_PixelSize[0], v_PixelSize[1]);
    s_PostShader->uniform2f("u_ScreenSize", v_ScreenSize[0], v_ScreenSize[1]);
    s_PostShader->uniform2f("u_SSAOScale",v_SSAOScale[0],v_SSAOScale[1]);
    s_PostShader->uniform1i("u_SuperSampling", 1);
    s_PostShader->uniform1f("u_Vignette", *v_Vignette);
    s_PostShader->uniform1f("u_Brightness", *v_PostBrightness);
    s_PostShader->uniform1i("u_FXAA", *v_FXAA);
    s_PostShader->uniform1f("u_Gamma", *v_Gamma);
    s_PostShader->uniform1i("u_DrawingMode", *v_DrawingMode);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

void PostPass::onResize(int width, int height) {
    log("post pass resize\n");
}

InputProcessResult PostPass::PostPassInputReceiver::onInputEvent(InputEvent *event){
    if (event->keyAction==QECore::ACTION_DOWN) {
        return INPUT_NOT_PROCESSED;
    }
    switch (event->keyCode) {
        case KEY_E: {
            *(parent->v_Vignette) += 0.1f;
            break;
        }
        case KEY_Q: {
            *(parent->v_Vignette) -= 0.1f;
            break;
        }
        case KEY_MINUS: {
            *(parent->v_DrawingMode) = 0;
            break;
        }
        case KEY_0: {
            *(parent->v_DrawingMode) = 0;
            break;
        }
        case KEY_1: {
            *(parent->v_DrawingMode) = 1;
            break;
        }
        case KEY_COMMA: {
            *(parent->v_Gamma) += 0.1f;
            break;
        }
        case KEY_SEMICOLON: {
            *(parent->v_Gamma) -= 0.1f;
            break;
        }
        case KEY_2: {
            *(parent->v_DrawingMode) = 2;
            break;
        }
        case KEY_3: {
            *(parent->v_DrawingMode) = 3;
            break;
        }
        case KEY_4: {
            *(parent->v_DrawingMode) = 4;
            break;
        }
        case KEY_5: {
            *(parent->v_DrawingMode) = 5;
            break;
        }
        /*case KEY_0 | KEY_1 | KEY_2 | KEY_3 | KEY_4 : {
            *(parent->v_DrawingMode) = event->keyCode - 47;
            break;
        }*/
        default:{break;}
    }
    return INPUT_PROCESSED;
}