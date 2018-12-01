//
// Created by ilya on 9/26/18.
//

#include "framebuffer.h"
#include "defines.h"
#include "util/log.h"
#include "opengl.h"

using namespace QECore;

Framebuffer::Framebuffer(){
    glGenFramebuffers(1, &pointer);
    log("generated new framebuffer with pointer = %u\n",pointer);
    bindFramebuffer();
}

Framebuffer::~Framebuffer() = default;

int Framebuffer::bindFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, pointer);
    return 0;
}

int Framebuffer::commitAttachments(unsigned *attachments, int size){
    glDrawBuffers(size,attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 0;
}

int Framebuffer::attachDepthmap(QECore::Texture *texture){
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getPointer(), 0);
    return 0;
}

int Framebuffer::attach(int attachment, QECore::Texture *t) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment, GL_TEXTURE_2D, t->getPointer(), 0);
    return 0;
}

int Framebuffer::attach(QECore::Texture *t)  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t->getPointer(), 0);
    return 0;
}

int Framebuffer::free()  {
    if (!pointer) return -1;
    glDeleteFramebuffers(1, &pointer);
    return 0;
}

