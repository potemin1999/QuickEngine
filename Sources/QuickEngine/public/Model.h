#pragma once

#include "Mesh.h"

class Model {
public:
    Mesh *meshes;
    int meshCount;

    Model(Mesh *meshes, int meshCount);

    Model() {};
};

