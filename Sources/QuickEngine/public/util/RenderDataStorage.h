#pragma once

#include <map>

namespace QE {

    class RenderDataStorage {
    private:

        /**
         * map<const char *, void *>
         * save cameras   c_
         *      variables v_
         *      textures  t_
         *      worlds    w_
         */
        std::map<std::string, void *> *data;

    public:

        RenderDataStorage();

        ~RenderDataStorage();

        int set(std::string, void *value);

        void *get(std::string);

        void *require(std::string);

    };

}