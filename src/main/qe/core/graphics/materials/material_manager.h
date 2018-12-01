#pragma once

#include <qe/core/graphics/basic/material.h>

using namespace QECore;

class MaterialManager {
public:

    static Material create_material(char *ambient);

    static Material create_material(char *ambient, char *diffuse);

    static Material *load_material(char *file, const char *name, unsigned int *count);

};
