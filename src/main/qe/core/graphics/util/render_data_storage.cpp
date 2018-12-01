//
// Created by ilya on 9/26/18.
//

#include <qe/core/graphics/util/render_data_storage.h>

#include <qe/core/util/log.h>

using namespace QECore;


RenderDataStorage::RenderDataStorage() {
    data = new StrMap<void*>();
}

RenderDataStorage::~RenderDataStorage() {
    delete (StrMap<void*> *) data;
}

int RenderDataStorage::set(const char * key,void* value) {
    log("written key : %s ; with value %p\n", key);
    (*(StrMap<void*>*) data)[key] = value;
}

void *RenderDataStorage::get(const char* key) {
    void* ret = (*(StrMap<void*>*) data)[key];
    log("required key %s ; returned %p\n", key, ret);
    return ret;
}

void *RenderDataStorage::require(const char *key) {
    void* ret = get(key);
    if (ret==nullptr){
        log("REQUIRED VALUE NOT FOUND : [key] = %d\n",key);
        exit(0);
    }
    return ret;
}
