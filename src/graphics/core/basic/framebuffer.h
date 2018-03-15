//
// Created by Ilya on 3/15/2018.
//

#ifndef QUICKENGINE_FRAMEBUFFER_H
#define QUICKENGINE_FRAMEBUFFER_H

#include "graphics/defines.h"
#include "texture.h"

namespace QEGraphics {

    class Framebuffer {

    private:

        //static Framebuffer *current;
        unsigned int pointer;

        inline int bindFramebuffer() {
            //if (current == this) return 1;
            glBindFramebuffer(GL_FRAMEBUFFER, pointer);
            //current = this;
            return 0;
        }

    public:

        Framebuffer() {
            glGenFramebuffers(1, &pointer);
            log("generated new framebuffer with pointer = %u\n",pointer);
            bindFramebuffer();
        }

        ~Framebuffer() {

        }

        inline int use() {
            return bindFramebuffer();
        }

        int commitAttachments(unsigned *attachments,int size){
            glDrawBuffers(size,attachments);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        int attachDepthmap(Texture* texture){
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getPointer(), 0);
            texture->onAttach(this);
            return 0;
        }

        int attach(int attachment, Texture *t) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, t->getPointer(), 0);
            t->onAttach(this);
            return 0;
        }

        int attach(Texture *t) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t->getPointer(), 0);
            t->onAttach(this);
            return 0;
        }

    };
}


#endif //QUICKENGINE_FRAMEBUFFER_H
