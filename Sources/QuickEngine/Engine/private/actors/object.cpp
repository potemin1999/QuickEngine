//
// Created by ilya on 9/26/18.
//

#include "object.h"

Object::Object() {
    mModelMatrix = mat4(1.0f);
}

Object::~Object() {
    mesh_count = 0;
    delete meshes;
}

void *Object::getModelMatrix() {
    return &mModelMatrix;
}
