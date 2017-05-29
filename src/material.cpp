#include "engine.h"
#include "material.h"


Material MH::create_material(char* ambient) {
	Material m = Material();
	*m.t_Ambient = TH::load_texture(ambient);
	return m;
}

Material MH::create_material(char* ambient, char* normal){
	Material m = Material();
    *m.t_Ambient = TH::load_texture(ambient);
	*m.t_Normal = TH::load_texture(normal);
	return m;
}

Material* MH::load_material(char* path,const char* name, unsigned int* mat_count){
	vector<Material> materials;
	string line;

    char path_c[1024];
	_getcwd(path_c, sizeof(path_c));
	string _path = string(path_c);
	_path.append("\\materials\\");
	_path.append(path);
	_path.append(name);

	string file_n = _path;//string(path) + string(name);


	ifstream file(file_n.c_str());

	if (!file) {
		printf("couldn't open material file \"%s\"\n", file_n.c_str());
		return 0;
	}
	else {
		printf("  loading material lib from file %s\n", file_n.c_str());
	}
	string hp_path = string(path_c).append("\\textures\\") + string(path);
	string* t = new string[16];

	//Material* mat_g;
	while (!file.eof()) {
		getline(file, line);

		if (line.find("#") == 0) {

		}else if (line.find("map_Bump") == 0) {
			split(line, ' ', t);
			//Texture* t_normal =
			*materials[materials.size() - 1].t_Normal = TH::load_texture((char*)(hp_path + t[1]).c_str());
			printf("      loaded normal map to id %i\n", materials[materials.size() - 1].t_Normal->id);
		}else if (line.find("map_Kd") == 0) {
			split(line, ' ', t);
			*materials[materials.size() - 1].t_Ambient = TH::load_texture((char*)(hp_path + t[1]).c_str());
			printf("      loaded diffuse map as ambient to id %i\n", materials[materials.size() - 1].t_Ambient->id);
		}else if (line.find("map_Ka") == 0) {
			split(line, ' ', t);
			*materials[materials.size() - 1].t_Ambient = TH::load_texture((char*)(hp_path + t[1]).c_str());
			printf("      loaded ambient map to id %i\n", materials[materials.size() - 1].t_Ambient->id);
		}else if (line.find("newmtl") == 0) {
			string* mtl_name = new string[2];
			split(line, ' ', mtl_name);
			Material m = Material();
			m.t_Ambient = new Texture;
			m.t_Normal = new Texture;
			char* name = (char*)mtl_name[1].c_str();
			printf("    newmtl %s\n", name);
			m.name = (char*) mtl_name[1].c_str();
			materials.push_back(m);
		}
		else {
		}
		//for (int i = 0; i < materials.size(); i++) {
		//	printf(" inline: material[%i] %s\n",i, (char*) materials[i]->name);
		//}
	}
	printf("  material lib size is %i\n", materials.size());
	*mat_count = materials.size();
	//for (int i = 0; i < materials.size(); i++) {
	//	printf("    material %s\n", (char*)materials[i]->name);
	//}
	Material* mat_arr = new Material[materials.size()];
	for (int i = 0;i<materials.size();i++)
        *(mat_arr + i) = materials[i];
	return mat_arr;
}
