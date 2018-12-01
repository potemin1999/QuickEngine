//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_FRAMEBUFFER_H
#define QUICKENGINE_FRAMEBUFFER_H

#include "texture.h"

namespace QECore {

    class Framebuffer {

    private:

        unsigned int pointer;

        int bindFramebuffer();

    public:

        Framebuffer();

        ~Framebuffer();

        inline int use() {
            return bindFramebuffer();
        }

        int commitAttachments(unsigned *attachments, int size);

        int attachDepthmap(Texture *texture);

        int attach(int attachment, Texture *t);

        int attach(Texture *t);

        int free();

    };
}


#endif //QUICKENGINE_FRAMEBUFFER_H
