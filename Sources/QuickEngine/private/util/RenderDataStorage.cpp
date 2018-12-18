//
// Created by ilya on 9/26/18.
//

#include "util/RenderDataStorage.h"
#include "util/Log.h"

using namespace QE;


RenderDataStorage::RenderDataStorage() {
    data = new std::map<std::string, void *>;
}

RenderDataStorage::~RenderDataStorage() {
    delete data;
}

int RenderDataStorage::set(std::string key, void *value) {
    printf("written key : %s; with value %p\n", key.c_str(), value);
    (*data)[key] = value;
}

void *RenderDataStorage::get(std::string key) {
    void *ret = (*data)[key];
    printf("requested key %s; returned %p\n", key.c_str(), ret);
    return ret;
}

void *RenderDataStorage::require(std::string key) {
    void *ret = get(key);
    if (ret == nullptr) {
        printf("REQUIRED VALUE NOT FOUND : [key] = %s\n", key.c_str());
        exit(0);
    }
    return ret;
}
