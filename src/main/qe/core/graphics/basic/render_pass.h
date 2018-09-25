//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_RENDER_PASS_H
#define QUICKENGINE_RENDER_PASS_H

namespace QEGraphics{

    class RenderPass{

    public:
        RenderPass() {}

        virtual ~RenderPass() {}
        virtual void compileShaders(){}
        virtual void onResize(int width,int height){}

        virtual void doDraw() {}

    };
}
#endif //QUICKENGINE_RENDER_PASS_H
