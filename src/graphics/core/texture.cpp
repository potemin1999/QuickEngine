#include "engine.h"
#include "texture.h"
#include "il\il.h"
#include "il\ilu.h"

Texture TH::load_texture(char* full_filename) {
	unsigned* il_id = new unsigned;
	unsigned* gl_id = new unsigned;
	ilGenImages(1, il_id);
	ilBindImage(*il_id);
	ilLoadImage(full_filename);
	Engine::check_gl_error("il gen/bind/load texture");
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
	float* f_max = new float;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, f_max);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);

	Engine::check_gl_error("parameter texture");
	glTexImage2D(GL_TEXTURE_2D,0,ilGetInteger(IL_IMAGE_FORMAT),ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),
		0,ilGetInteger(IL_IMAGE_FORMAT),GL_UNSIGNED_BYTE,ilGetData());

    log("%i",ilGetInteger(IL_IMAGE_FORMAT));
    Engine::check_gl_error("tex image 2d texture");
	//glGenerateMipmap(GL_TEXTURE_2D);
	Texture t = Texture();
	//t->source = new char[string(full_filename).length()];
	t.width = ilGetInteger(IL_IMAGE_WIDTH);
	t.height = ilGetInteger(IL_IMAGE_HEIGHT);
	t.source = full_filename;
	t.id = *gl_id;
	Engine::check_gl_error("load texture");
	return t;
}

int TH::use_texture(Texture* texture) {
	ilBindImage(texture->id);
}

int TH::free_texture(Texture* texture) {
	ilBindImage(0);
	ilDeleteImage(texture->id);
}
