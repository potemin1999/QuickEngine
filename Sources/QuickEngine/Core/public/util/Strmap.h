#pragma once

#include <map>

/*#ifdef __linux__
#define STORAGE_KEY_TYPE const char*
#endif
#ifdef _WIN32*/
#include <string>

#define STORAGE_KEY_TYPE std::string
//#endif

namespace QE {

    namespace StrMapImpl {
        typedef STORAGE_KEY_TYPE Key;
    }

    using namespace StrMapImpl;

    template<typename T>
    class StrMap {

        std::map<Key, T> data;

    public:
        T &operator[](const char *k) {
//#ifdef _WIN32
            std::string sKey(k);
            return data[sKey];
//#else
            //           return data[k];
///#endif
        }

        void remove(const char *k) {
//#ifdef _WIN32
            std::string sKey(k);
            data.erase(sKey);
//#else
            //          data.erase(k);
//#endif
        }

    };

}