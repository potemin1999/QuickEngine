#include "defines.h"
#include "engine.h"
#include <fstream>
#include <unistd.h>
#include "util/string_helper.h"
#include "util/utils.h"

using namespace QECore;

Material MaterialManager::create_material(char *ambient) {
    Material m = Material();
    *m.t_Ambient = TextureManager::loadTexture(ambient);
    return m;
}

Material MaterialManager::create_material(char *ambient, char *normal) {
    Material m = Material();
    *m.t_Ambient = TextureManager::loadTexture(ambient);
    *m.t_Normal = TextureManager::loadTexture(normal);
    return m;
}

Material *MaterialManager::load_material(char *path, const char *name, unsigned int *mat_count) {
    vector<Material> materials;
    string line;

    char path_c[1024];
    QECore::getEngineRootDir(path_c, sizeof(path_c));
    string _path = string(path_c);
    _path.append("/materials/");
    _path.append(path);
    _path.append(name);

    string file_n = _path;//string(path) + string(name);


    ifstream file(file_n.c_str());

    if (!file) {
        printf("couldn't open material file \"%s\"\n", file_n.c_str());
        return 0;
    } else {
        log("  loading material lib from file %s\n", file_n.c_str());
    }
    string hp_path = string(path_c).append("/textures/") + string(path);
    auto t = new string[16];
    string mtl_name[2];
    //Material* mat_g;
    while (!file.eof()) {
        getline(file, line);

        if (line.find('#') == 0) {

        } else if (line.find("map_Bump") == 0) {
            split(line, ' ', t);
            //Texture* t_normal =
            *materials[materials.size() - 1].t_Normal = TextureManager::loadTexture((char *) (hp_path + t[1]).c_str());
            log("      loaded normal map to id %i\n", materials[materials.size() - 1].t_Normal->id);
        } else if (line.find("map_Kd") == 0) {
            split(line, ' ', t);
            *materials[materials.size() - 1].t_Ambient = TextureManager::loadTexture((char *) (hp_path + t[1]).c_str());
            log("      loaded diffuse map as ambient to id %i\n", materials[materials.size() - 1].t_Ambient->id);
        } else if (line.find("map_Ka") == 0) {
            split(line, ' ', t);
            *materials[materials.size() - 1].t_Ambient = TextureManager::loadTexture((char *) (hp_path + t[1]).c_str());
            log("      loaded ambient map to id %i\n", materials[materials.size() - 1].t_Ambient->id);
        } else if (line.find("newmtl") == 0) {
            split(line, ' ', mtl_name);
            Material m = Material();
            m.t_Ambient = new Texture();
            m.t_Normal = new Texture();
            auto cName = (char *) mtl_name[1].c_str();
            m.name = new char[strlen(cName)];
            strcpy(m.name, cName);
            log("    newmtl %s\n", m.name);
            materials.push_back(m);
        } else {
        }
        //for (int i = 0; i < materials.size(); i++) {
        //	printf(" inline: material[%i] %s\n",i, (char*) materials[i]->name);
        //}
    }
    delete[] t;
    log("  material lib size is %i\n", materials.size());
    *mat_count = (unsigned int) materials.size();
    //for (int i = 0; i < materials.size(); i++) {
    //	printf("    material %s\n", (char*)materials[i]->name);
    //}
    auto mat_arr = new Material[materials.size()];
    for (int i = 0; i < materials.size(); i++)
        *(mat_arr + i) = materials[i];
    file.close();
    return mat_arr;
}
