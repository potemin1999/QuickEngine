//
// Created by ilya on 9/26/18.
//

#include "util/utils.h"
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <random>
#include "../../public/util/utils.h"


#ifdef __linux__

char *(*__getcwd)(char *, size_t) = getcwd;

#endif
#ifdef _WIN32
char* (*__getcwd)(char*,int) = getcwd;
#endif

float QECore::random(float min, float max) {
    static std::mt19937 *engine;
    if (!engine) {
        std::random_device rd;
        engine = new std::mt19937(rd());
        time_t tt;
        engine->seed((unsigned) time(&tt));
    }
    std::uniform_real_distribution<float> dis(min, max);
    return dis(*engine);
    /*
#ifdef __linux__
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<float> dis(min,max);
    return dis(engine);
#endif
#ifdef _WIN32
    const float uintDiv = (1.0 / RAND_MAX);
    //std::random_device rd;
    //std::mt19937 engine(rd());
   // time_t tim;

   // engine.seed(time);
    //std::uniform_int_distribution<uint32_t> dis(0,UINT32_MAX);
    float ret = (max-min)* (rand() * uintDiv) + min;
    //printf("generating random : %u\n",dis(engine));
    return ret;
#endif*/
}

void QECore::getcwd(char *buffer, int size) {
#ifdef __linux__
    (*__getcwd)(buffer, (size_t) size);
#endif
#ifdef _WIN32
    (*__getcwd)(buffer,size);
#endif
}

void QECore::getEngineRootDir(char *buffer, int size) {
    getcwd(buffer, size);
    auto length = strlen(buffer);
    auto last_slash = -1;
    for (auto i = length; i >= 0; --i) {
        if (  /**start if*/
#ifndef _WIN32
            buffer[i] == '/'
#else
            buffer[i] == '\\'
#endif
            /**end if*/) {
            last_slash = (int) i;
            break;
        }else{
            buffer[i] = 0;
        }
    }
    if (last_slash == -1)
        return;
    buffer[last_slash] = '\0';
}
