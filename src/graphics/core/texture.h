#pragma once

#include "graphics/defines.h"
#include "settings.h"

struct TextureT {
	int id;
	char* source;
	int width, height;
} typedef TextureT;

class TH {
public:
	static Texture load_texture(char* filename);
	static int use_texture(Texture* texture);
	static int free_texture(Texture* texture);
};