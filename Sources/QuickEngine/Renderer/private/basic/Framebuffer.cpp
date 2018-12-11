//
// Created by ilya on 9/26/18.
//

#include "Framebuffer.h"
#include "Defines.h"
#include "util/Log.h"
#include "OpenGL.h"

using namespace QE;

Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &pointer);
    log("generated new framebuffer with pointer = %u\n", pointer);
    bindFramebuffer();
}

Framebuffer::~Framebuffer() = default;

int Framebuffer::bindFramebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, pointer);
    return 0;
}

int Framebuffer::commitAttachments(unsigned *attachments, int size) {
    glDrawBuffers(size, attachments);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 0;
}

int Framebuffer::attachDepthmap(QE::Texture *texture) {
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture->getPointer(), 0);
    return 0;
}

int Framebuffer::attach(int attachment, QE::Texture *t) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum) attachment, GL_TEXTURE_2D, t->getPointer(), 0);
    return 0;
}

int Framebuffer::attach(QE::Texture *t) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, t->getPointer(), 0);
    return 0;
}

int Framebuffer::free() {
    if (!pointer) return -1;
    glDeleteFramebuffers(1, &pointer);
    return 0;
}

