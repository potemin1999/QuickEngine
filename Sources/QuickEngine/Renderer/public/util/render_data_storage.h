//
// Created by ilya on 9/26/18.
//

#ifndef QUICKENGINE_RENDER_DATA_STORAGE_H
#define QUICKENGINE_RENDER_DATA_STORAGE_H

namespace QECore {

    class RenderDataStorage {
    private:

        /**
         * map<const char*,void*>
         * save cameras   c_
         *      variables v_
         *      textures  t_
         *      worlds    w_
         */
        void *data;

    public:

        RenderDataStorage();

        ~RenderDataStorage();

        int set(const char *key, void *value);

        void *get(const char *key);

        void *require(const char* key);

    };

}

#endif //QUICKENGINE_RENDER_DATA_STORAGE_H
