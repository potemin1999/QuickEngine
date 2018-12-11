#pragma once

#include "Engine.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "il/il.h"

#define WINDOW_CREATING_ERROR 0x89089089

extern GLFWwindow *window;
extern QuickEngine *engine;

int runEngine();

extern int mainGraphics();

extern void resize(GLFWwindow *w, int width, int height);

extern void key(GLFWwindow *window, int key, int scancode, int action, int mods);

extern void error(int error, const char *description);

extern void init();

extern void loop();