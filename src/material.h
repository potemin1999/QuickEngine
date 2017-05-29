#pragma once

#include <string>
#include <fstream>
#include <list>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <vector>
#include <io.h>
#include "texture.h"
#include "string_helper.h"

struct Material {
	char* name;
	Texture* t_Ambient;
	Texture* t_Normal;
} typedef Material;

class MH {
//private:
	/*
	static void chop(string &str) {
		string spaces(" \t\f\v\n\r");
		unsigned int found = str.find_last_not_of(spaces);
		if (found != string::npos) {
			str.erase(found + 1);
		}
		else {
			str.clear();
		}
	}

	static void split(string &str, char d, string* tokens) {
		stringstream iss(str);
		string item;
		int i = 0;

		while (getline(iss, item, d)) {
			if (item != " " && !item.empty()) {
				chop(item);
				tokens[i] = item;
				i++;
			}
		}
	}*/
public:
	static Material create_material(char* ambient);
	static Material create_material(char* ambient, char* diffuse);
	static Material* load_material(char* file,const char* name, unsigned int* count);
};
