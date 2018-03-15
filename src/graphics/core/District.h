#pragma once
#include <list>
#include "Object.h"

class District {
public:
	bool to_render = true;
	float xyz[3];
	list<Object*> objects;
};
