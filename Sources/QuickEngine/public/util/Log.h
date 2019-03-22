//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_LOG_H
#define QUICKENGINE_LOG_H

inline void log(const char *format, ...) {
#ifdef LOG
    va_list args;
    va_start(args, format);
    /*const char* prefix = "[GRAPHICS]:";
    char* newFormat = new char[strlen(prefix)+strlen(format)];
    strcpy(newFormat,prefix);
    strcpy(newFormat+strlen(prefix),format);*/
    vprintf(format, args);
    va_end(args);
#endif
}

#endif //QUICKENGINE_LOG_H
