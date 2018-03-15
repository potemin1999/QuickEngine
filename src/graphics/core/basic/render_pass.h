//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_RENDER_PASS_H
#define QUICKENGINE_RENDER_PASS_H

#include "graphics/core/engine.h"

using namespace QEGraphics;

namespace QEGraphics{

    class RenderPass{

        virtual void compileShaders(){}
        virtual void onResize(int width,int height){}
        virtual void draw(){}

    };
}
#endif //QUICKENGINE_RENDER_PASS_H
