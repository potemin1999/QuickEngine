#pragma once

#include <stdio.h>
//#include <tchar.h>


#include "glad\glad.h"
#include "glfw\glfw3.h"

//#include <gl\GLU.h>

#include "engine.h"
#include "il\il.h"
//#include "il\ilu.h"

#include "settings.h"

#define WINDOW_CREATING_ERROR 0x89089089

int multisampling = 1;
int window_width=1600;
int window_height=1000;
int main(void);
void resize(GLFWwindow* w, int width, int height);
void key(GLFWwindow* window, int key, int scancode, int action, int mods);
void error(int error, const char* description);
void init();
void loop();
