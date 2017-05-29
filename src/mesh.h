#pragma once
#include "material.h"

struct Mesh {
	Material* material;
	int vert_count;
	float xyz[3];
	float frustum_radius = 1.0;
	int data;
	int vbo;
	int tbo;
	int nbo;
	bool to_draw = true;
	bool loaded = false;
	char* name="unnamed";
};