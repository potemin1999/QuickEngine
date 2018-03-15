//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_SSAO_SHADER_H
#define QUICKENGINE_SSAO_SHADER_H

#include "graphics/core/basic/shader.h"

namespace QEGraphics{

    class SSAOShader: Shader{

    public:
        SSAOShader() : Shader("shaders/ssao_vertex_shader.glsl","shaders/ssao_fragment_shader.glsl"){

        }
    };
}

#endif //QUICKENGINE_SSAO_SHADER_H
