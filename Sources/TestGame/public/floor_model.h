#pragma once

#include "defines.h"
#include "object.h"
#include "model_loader.h"

class FloorModel : public Object {
public:
	FloorModel() : Object() {
        ModelLoader::load_object(this, "", "floor_01.obj");
	};
};