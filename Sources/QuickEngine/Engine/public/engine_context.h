//
// Created by ilya on 9/24/18.
//

#ifndef QUICKENGINE_ENGINE_CONTEXT_H
#define QUICKENGINE_ENGINE_CONTEXT_H

#include "input/input_event_manager.h"

using namespace QECore;

namespace QECore {
    
    class EngineContext {
    public:

        EngineContext() = default;
        virtual ~EngineContext() = default;
        virtual InputEventManager *getInputEventManager() = 0;
        
    };
    
}
#endif //QUICKENGINE_ENGINE_CONTEXT_H
