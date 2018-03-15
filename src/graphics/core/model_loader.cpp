#include "model_loader.h"

void ML::load_object(Object* container,char* path,char* name) {
	log("model loading from file %s\n", path);
	int vertex_count = 0;
	float p_scale = 0.05f;
	float n_scale = 1.0f;

    char path_c[1024];
	_getcwd(path_c, sizeof(path_c));
	string _path = string(path_c);
	_path.append("\\models\\");
	_path.append(path);
	_path.append(name);
	log("model loading from file %s\n", _path.data());
	ifstream file(_path);//"models\\"+string(path)+string(name));
	string just_path = string(path_c).append(path);

	if (!file) {
		printf("couldn't open model file \"models\\%s%s\"\n",_path.data());// string(path).c_str(), string(name).c_str());
		return;
	}
	struct vert {
		float xyz[3];
	};
	struct tex {
		float uv[2];
	};
	struct norm {
		float xyz[3];
	};
	vector<vert> vertices;
	vector<tex>  textures;
	vector<norm> normales;
	vector<Mesh> meshes;
	vector<Material> materials;
	vector<float> out_array;
	string line;

	Material* last_mat = nullptr;

	//Mesh* m = new Mesh();
	//meshes.push_back(*m);

	while (!file.eof()) {
		getline(file, line);
		if (line.find("#") == 0) {

		}else if (line.find("vt") == 0) {
			string tokens[3];
			split(line, ' ', tokens);
			tex t;
			t.uv[0] = strtof(tokens[1].c_str(), 0);
			t.uv[1] = 1 - strtof(tokens[2].c_str(), 0);
			textures.push_back(t);
		}else if (line.find("vn") == 0) {
			string tokens[4];
			split(line, ' ', tokens);
			norm n;
			n.xyz[0] = n_scale*strtof(tokens[1].c_str(), 0);
			n.xyz[1] = n_scale*strtof(tokens[2].c_str(), 0);
			n.xyz[2] = n_scale*strtof(tokens[3].c_str(), 0);
			normales.push_back(n);
		}else if (line.find("v") == 0) {
			string tokens[4];
			split(line, ' ', tokens);
			vert v;
			v.xyz[0] = p_scale*strtof(tokens[1].c_str(), 0);
			v.xyz[1] = p_scale*strtof(tokens[2].c_str(), 0);
			v.xyz[2] = p_scale*strtof(tokens[3].c_str(), 0);
			vertices.push_back(v);
		}else if (line.find("mtllib") == 0) {
			string* mtl_path = new string[16];
			split(line, ' ', mtl_path);
			string* mtl = new string(mtl_path[1]);
			unsigned int* mat_size = new unsigned int;
			//string m_path = string(path_c).append("\\materials\\").append(path);
			Material* mat = MH::load_material(path,mtl->c_str(),mat_size);
			for (unsigned int i = 0; i < *mat_size; i++) {
				//printf("    i = %u, name = %s\n", i, (mat + i)->name);
				materials.push_back( *(mat + i));
			}
#ifdef GRAPHICS_DEBUG
			for (unsigned int i = 0; i < materials.size(); i++) {
				log("    i = %u, name = %s\n", i, materials[i].name);
			}
			log("  materials list size : %i\n", materials.size());
#endif
		}else if (line.find("usemtl") == 0) {
			string* mtl_path = new string[16];
			split(line, ' ', mtl_path);
			//string* mtl = new string(mtl_path[1]);
			char* mtl_name = (char*) mtl_path[1].c_str();
			for (int i = 0; i < materials.size(); i++) {
				if ( strcmp( materials[i].name, mtl_name)==0) {
					last_mat = &materials[i];
				}
			}
		}else if (line.find("g ") == 0 || line.find("s ") == 0) {
			if (!out_array.empty()) {
				Mesh& last = meshes[meshes.size() - 1];
				last.vert_count = vertex_count;
				EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), last.data);
				log("    data created to link %i\n", last.data);
				out_array.clear();
				vertex_count = 0;
			}
			string* mesh_n = new string[2];
			Mesh m;
			split(line, ' ', mesh_n);
			string* mesh_name = new string(mesh_n[1]);
			m.name = (char*)mesh_name->c_str();
			m.material = new Material;
			*m.material = *last_mat;// &materials[materials.size() - 1];
			//last_mat = &materials[materials.size() - 1];
			meshes.push_back(m);
			log("    creating mesh group %s, uses material %s\n", m.name , (last_mat ? last_mat->name : "nullptr") );
		}else if (line.find("o") == 0) {

		}else if (line.find("f") == 0) {
			string tokens[4];
			split(line, ' ', tokens);
			//printf("created polygon\n");
			for (int i = 1; i < 4; i++) {

				string items[3];

				split(tokens[i], '/', items);
				int in[3];
				in[0] = atoi(items[0].c_str()) - 1;
				in[1] = atoi(items[1].c_str()) - 1;
				in[2] = atoi(items[2].c_str()) - 1;
				vert v = vertices[in[0]];
				tex  t = textures[in[1]];
				norm n = normales[in[2]];

				for (int i = 0; i < 3; i++)
					out_array.push_back(v.xyz[i]);
				for (int i = 0; i < 2; i++)
					out_array.push_back(t.uv[i]);
				for (int i = 0; i < 3; i++)
					out_array.push_back(n.xyz[i]);
				vertex_count++;
				//printf("    v: %f %f %f t : %f %f n : %f %f %f\n", v.xyz[0], v.xyz[1], v.xyz[2], t.uv[0], t.uv[1], n.xyz[0], n.xyz[1], n.xyz[2]);
			}
			//printf("end of polygon\n\n");
		}
	}
	if (!out_array.empty()) {
		if (meshes.size() == 0) {
			container->meshes = new Mesh();
		}
		else {
			Mesh& last = meshes[meshes.size() - 1];
			last.vert_count = vertex_count;
			EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), last.data);
			log("    data created to link %i\n", last.data);

			out_array.clear();
		}
	}
	container->meshes = new Mesh[meshes.size()];
	container->mesh_count = meshes.size();
	for (int i = 0; i < container->mesh_count; i++) {
		*(container->meshes + i) = meshes[i];
	}
	//container->mesh_count = meshes.size();
	//printf("size of v:%i t:%i n:%i", vertices.size(), textures.size(), normales.size());
	//EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), vao);
	log("done\n");
}


void ML::load_OBJ(char* path, int &vao, int &vertex_count) {

	vertex_count = 0;
	float p_scale = 0.5f;
	float n_scale = 0.5f;

    char path_c[1024];
	_getcwd(path_c, sizeof(path_c));
	string _path = string(path_c);
	_path.append("\\");
	_path.append(path);
	ifstream file(_path);

	if (!file) {
		printf("Couldn't open model file (OBJ) \"%s\"\n", _path.data());
		return;
	}
	struct vert {
		float xyz[3];
	};
	struct tex {
		float uv[2];
	};
	struct norm {
		float xyz[3];
	};
	vector<vert> vertices;
	vector<tex>  textures;
	vector<norm> normales;
	vector<float> out_array;

	string line;


	while (!file.eof()) {
		getline(file, line);
		if (line.find("#") == 0) {

		}
		else if (line.find("vt") == 0) {

			string tokens[3];
			split(line, ' ', tokens);
			tex t;
			t.uv[0] = strtof(tokens[1].c_str(), 0);
			t.uv[1] = 1 - strtof(tokens[2].c_str(), 0);
			textures.push_back(t);


		}
		else if (line.find("vn") == 0) {

			string tokens[4];
			split(line, ' ', tokens);
			norm n;
			n.xyz[0] = n_scale*strtof(tokens[1].c_str(), 0);
			n.xyz[1] = n_scale*strtof(tokens[2].c_str(), 0);
			n.xyz[2] = n_scale*strtof(tokens[3].c_str(), 0);
			normales.push_back(n);

		}
		else if (line.find("v") == 0) {

			string tokens[4];
			split(line, ' ', tokens);
			vert v;
			v.xyz[0] = p_scale*strtof(tokens[1].c_str(), 0);
			v.xyz[1] = p_scale*strtof(tokens[2].c_str(), 0);
			v.xyz[2] = p_scale*strtof(tokens[3].c_str(), 0);
			vertices.push_back(v);
		}
		else if (line.find("mtllib") == 0) {

		}
		else if (line.find("usemtl") == 0) {

		}
		else if (line.find("g") == 0) {

		}
		else if (line.find("o") == 0) {

		}
		else if (line.find("f") == 0) {


			string tokens[4];
			split(line, ' ', tokens);
			//printf("created polygon\n");
			for (int i = 1; i < 4; i++) {

				string items[3];

				split(tokens[i], '/', items);
				int in[3];
				in[0] = atoi(items[0].c_str()) - 1;
				in[1] = atoi(items[1].c_str()) - 1;
				in[2] = atoi(items[2].c_str()) - 1;
				vert v = vertices[in[0]];
				tex  t = textures[in[1]];
				norm n = normales[in[2]];

				for (int i = 0; i < 3; i++)
					out_array.push_back(v.xyz[i]);
				for (int i = 0; i < 2; i++)
					out_array.push_back(t.uv[i]);
				for (int i = 0; i < 3; i++)
					out_array.push_back(n.xyz[i]);
				vertex_count++;
				//printf("    v: %f %f %f t : %f %f n : %f %f %f\n", v.xyz[0], v.xyz[1], v.xyz[2], t.uv[0], t.uv[1], n.xyz[0], n.xyz[1], n.xyz[2]);
			}
			//printf("end of polygon\n\n");
		}
	}
	printf("size of model %i polygons\n", vertex_count);
	EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), vao);
	printf("model loaded form file %s\n", path);


}
