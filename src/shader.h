#pragma once

struct Shader {
	int program_id = 0;
} typedef Shader;

class SH {
public:
	static Shader* create_shader_program(char* vertex_path, char* fragment_path);
	static int get_uniform(Shader* program, char* name);
	static void get_uniform(Shader* program, char* name, unsigned int* ref);
	static int load_vertex_shader(char* v);
	static int load_fragment_shader(char* f);
	static int link_shaders(int p_handle, int v_handle, int f_handle);
	static int load_shaders(char* v, char* f);
	static int use_shader_program(int program);
	static int use_shader_program(Shader* shader);
};
