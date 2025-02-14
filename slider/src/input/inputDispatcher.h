#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/utils/eventSource.h"
#include <functional>

namespace Input
{
    class InputDispatcher : public Utils::EventSource<const Event&>
    {
    public:
        void ProcessInput(const InputData& input);
    private:
        InputData m_LastInput;
    };
}

#endif