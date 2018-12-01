#include "qe/core/graphics/util/texture_manager.h"
#include "il/il.h"
#include "qe/core/graphics/defines.h"
#include "qe/core/settings.h"
#include "qe/core/engine.h"

using namespace QECore;

Texture TextureManager::loadTexture(char *full_filename) {
    auto *il_id = new unsigned;
    auto *gl_id = new unsigned;
    ilGenImages(1, il_id);
    ilBindImage(*il_id);
    printf("Loading texture from file \"%s\"\n", full_filename);
    ilLoadImage(full_filename);
    Engine::checkGlError("il gen/bind/load texture");
#ifdef GRAPHICS_DEBUG
    printf("loading texture %s",full_filename);
#endif
    if (!ilGetData()) {
        printf("loading texture named %s failed", full_filename);
    }
    glGenTextures(1, gl_id);
    glBindTexture(GL_TEXTURE_2D, *gl_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//_MIPMAP_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    auto *f_max = new float;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, f_max);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

    Engine::checkGlError("parameter texture");
    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH),
                 ilGetInteger(IL_IMAGE_HEIGHT),
                 0, (GLenum) ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

    log("%i", ilGetInteger(IL_IMAGE_FORMAT));
    Engine::checkGlError("tex image 2d texture");
    //glGenerateMipmap(GL_TEXTURE_2D);
    Texture t = Texture();
    //t->source = new char[string(full_filename).length()];
    t.width = ilGetInteger(IL_IMAGE_WIDTH);
    t.height = ilGetInteger(IL_IMAGE_HEIGHT);
    t.source = full_filename;
    t.id = *gl_id;
    Engine::checkGlError("load texture");
    delete il_id;
    delete gl_id;
    delete f_max;
    return t;
}
