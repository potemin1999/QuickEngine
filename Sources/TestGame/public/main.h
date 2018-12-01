#ifndef _QECORE_H_
#define _QECORE_H_

#include <stdio.h>
//#include <tchar.h>


#include "glad/glad.h"
#include "glfw/glfw3.h"

//#include <gl\GLU.h>

#include "engine.h"
#include "boot.h"
#include "il/il.h"

#include "max_model.h"
//#include "test_model.h"
#include "floor_model.h"
//#include "il\ilu.h"

#include "settings.h"

#define WINDOW_CREATING_ERROR 0x89089089

extern int multisampling;
extern int window_width;
extern int window_height;

extern int main_graphics();

extern void resize(GLFWwindow *w, int width, int height);

extern void key(GLFWwindow *window, int key, int scancode, int action, int mods);

extern void error(int error, const char *description);

extern void init();

extern void loop();

#endif