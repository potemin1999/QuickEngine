#include "model_loader.h"
#include "materials/material_manager.h"
#include "util/log.h"
#include "util/utils.h"
#include "engine.h"

void ModelLoader::load_object(GameObject *container, const char *path, const char *name) {
    ModelLoader::load_object(container, (char *) path, (char *) name);
}

void ModelLoader::load_object(GameObject *container, char *path, char *name) {
    log(".model loading from file %s\n", path);
    int vertex_count = 0;
    float p_scale = 1.0f;
    float n_scale = 1.0f;

    char path_c[1024];
    QECore::getEngineRootDir(path_c, sizeof(path_c));
    string _path = string(path_c);
    _path.append("/models/");
    _path.append(path);
    _path.append(name);
    log("model loading from file %s\n", _path.data());
    ifstream file(_path);//"models\\"+string(path)+string(name));
    string just_path = string(path_c).append(path);

    if (!file) {
        printf("couldn't open model file \"%s\"\n", _path.data());// string(path).c_str(), string(name).c_str());
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
    vector<tex> textures;
    vector<norm> normals;
    vector<Mesh> meshes;
    vector<Material> materials;
    vector<float> out_array;
    string line;

    Material *last_mat = nullptr;

    string tokens[4];
    string mtl_path[16];
    string items[3];
    while (!file.eof()) {
        getline(file, line);
        if (line.find('#') == 0) {

        } else if (line.find("vt") == 0) {
            split(line, ' ', tokens);
            tex t{};
            t.uv[0] = strtof(tokens[1].c_str(), nullptr);
            t.uv[1] = 1 - strtof(tokens[2].c_str(), nullptr);
            textures.push_back(t);
        } else if (line.find("vn") == 0) {
            split(line, ' ', tokens);
            norm n{};
            n.xyz[0] = n_scale * strtof(tokens[1].c_str(), nullptr);
            n.xyz[1] = n_scale * strtof(tokens[2].c_str(), nullptr);
            n.xyz[2] = n_scale * strtof(tokens[3].c_str(), nullptr);
            normals.push_back(n);
        } else if (line.find('v') == 0) {
            split(line, ' ', tokens);
            vert v{};
            v.xyz[0] = p_scale * strtof(tokens[1].c_str(), nullptr);
            v.xyz[1] = p_scale * strtof(tokens[2].c_str(), nullptr);
            v.xyz[2] = p_scale * strtof(tokens[3].c_str(), nullptr);
            vertices.push_back(v);
        } else if (line.find("mtllib") == 0) {
            split(line, ' ', mtl_path);
            string mtl = mtl_path[1];
            unsigned mat_size = 0;
            Material *mat = MaterialManager::load_material(path, mtl.c_str(), &mat_size);
            for (unsigned int i = 0; i < mat_size; i++) {
                materials.push_back(*(mat + i));
            }
            delete[] mat;
        } else if (line.find("usemtl") == 0) {
            split(line, ' ', mtl_path);
            auto mtl_name = (char *) mtl_path[1].c_str();
            for (auto &material : materials) {
                if (strcmp(material.name, mtl_name) == 0) {
                    last_mat = &material;
                }
            }
        } else if (line.find("g ") == 0 || line.find("s ") == 0) {
            if (!out_array.empty()) {
                Mesh &last = meshes[meshes.size() - 1];
                last.vert_count = vertex_count;
                EU::genVBOandVAO(&out_array[0], sizeof(out_array[0]) * out_array.size(), last.data);
                log("    data created to link %i\n", last.data);
                out_array.clear();
                vertex_count = 0;
            }
            string mesh_n[2];
            Mesh m;
            split(line, ' ', mesh_n);
            const char *c_mesh_name = mesh_n[1].c_str();
            auto *mesh_name = new char[strlen(c_mesh_name)];
            strcpy(mesh_name, c_mesh_name);
            m.name = mesh_name;
            m.material = new Material;
            *m.material = *last_mat;
            meshes.push_back(m);
            log("    creating mesh group %s, uses material %s\n", m.name, last_mat->name);
        } else if (line.find('o') == 0) {

        } else if (line.find('f') == 0) {
            split(line, ' ', tokens);
            for (int i = 1; i < 4; i++) {

                split(tokens[i], '/', items);
                vert &v = vertices[atoi(items[0].c_str()) - 1];
                tex &t = textures[atoi(items[1].c_str()) - 1];
                norm &n = normals[atoi(items[2].c_str()) - 1];

                for (float j : v.xyz)
                    out_array.push_back(j);
                for (float k : t.uv)
                    out_array.push_back(k);
                for (float m : n.xyz)
                    out_array.push_back(m);
                vertex_count++;
            }
        }
    }
    if (!out_array.empty()) {
        if (meshes.empty()) {
            container->meshes = new Mesh();
        } else {
            Mesh &last = meshes[meshes.size() - 1];
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
    file.close();
    log("done\n");
}


void ModelLoader::load_OBJ(char *path, int &vao, int &vertex_count) {

    vertex_count = 0;
    float p_scale = 0.5f;
    float n_scale = 0.5f;

    char path_c[1024];
    getcwd(path_c, sizeof(path_c));
    string _path = string(path_c);
    _path.append("/");
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
    vector<tex> textures;
    vector<norm> normales;
    vector<float> out_array;

    string line;


    while (!file.eof()) {
        getline(file, line);
        if (line.find('#') == 0) {

        } else if (line.find("vt") == 0) {

            string tokens[3];
            split(line, ' ', tokens);
            tex t;
            t.uv[0] = strtof(tokens[1].c_str(), nullptr);
            t.uv[1] = 1 - strtof(tokens[2].c_str(), nullptr);
            textures.push_back(t);


        } else if (line.find("vn") == 0) {

            string tokens[4];
            split(line, ' ', tokens);
            norm n;
            n.xyz[0] = n_scale * strtof(tokens[1].c_str(), nullptr);
            n.xyz[1] = n_scale * strtof(tokens[2].c_str(), nullptr);
            n.xyz[2] = n_scale * strtof(tokens[3].c_str(), nullptr);
            normales.push_back(n);

        } else if (line.find('v') == 0) {

            string tokens[4];
            split(line, ' ', tokens);
            vert v;
            v.xyz[0] = p_scale * strtof(tokens[1].c_str(), nullptr);
            v.xyz[1] = p_scale * strtof(tokens[2].c_str(), nullptr);
            v.xyz[2] = p_scale * strtof(tokens[3].c_str(), nullptr);
            vertices.push_back(v);
        } else if (line.find("mtllib") == 0) {

        } else if (line.find("usemtl") == 0) {

        } else if (line.find('g') == 0) {

        } else if (line.find('o') == 0) {

        } else if (line.find('f') == 0) {


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
                tex t = textures[in[1]];
                norm n = normales[in[2]];

                for (int j = 0; j < 3; j++)
                    out_array.push_back(v.xyz[j]);
                for (int k = 0; k < 2; k++)
                    out_array.push_back(t.uv[k]);
                for (int m = 0; m < 3; m++)
                    out_array.push_back(n.xyz[m]);
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
