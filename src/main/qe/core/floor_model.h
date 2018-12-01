#pragma once

#include "qe/core/graphics/defines.h"
#include "qe/core/actors/object.h"
#include "qe/core/actors/model_loader.h"
#include "engine.h"

class FloorModel : public Object {
public:
	FloorModel() : Object() {
        ModelLoader::load_object(this, "", "floor_01.obj");
	};
};