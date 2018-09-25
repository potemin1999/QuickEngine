//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_ENGINE_CONTEXT_H
#define QUICKENGINE_ENGINE_CONTEXT_H

#include <qe/core/input/input_event_manager.h>

using namespace QEInput;

namespace QECore {
    class EngineContext {
    public:
        virtual InputEventManager *getInputEventManager() = 0;
    };
}
#endif //QUICKENGINE_ENGINE_CONTEXT_H
