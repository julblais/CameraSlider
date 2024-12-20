#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/utils/eventSource.h"

namespace Input
{
    class InputDispatcher : public Utils::EventSource<IInputListener>
    {
        public:
            void SetInitialInput(const InputData& input);
            void ProcessInput(const InputData& input);

        private:            
            InputData m_LastInput;
    };
}

#endif