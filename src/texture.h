#pragma once
#include "settings.h"

struct Texture {
	int id;
	char* source;
	int width, height;
} typedef Texture;

class TH {
public:
	static Texture load_texture(char* filename);
	static int use_texture(Texture* texture);
	static int free_texture(Texture* texture);
};