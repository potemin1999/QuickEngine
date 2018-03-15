#pragma once

#include "graphics/defines.h"
#include <fstream>
#include <list>
#include <fstream>
#include <vector>
#ifdef GRAPHICS_DEBUG
#include <iostream>
#endif
#include <sstream>
#include <vector>
#include <cstring>
#include <io.h>
#include "engine.h"
#include "string_helper.h"

class ML {

//private:
	//static void chop(string &str);
	//static void split(string &str, char d, string* tokens);


public:
	static void load_object(Object* container,char* path,char* name);
	static void load_OBJ(char* path, int &vao, int &vertex_count);




};
