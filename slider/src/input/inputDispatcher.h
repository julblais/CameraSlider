#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/utils/eventSource.h"

namespace Input
{
    class InputDispatcher : public Utils::EventSource<IInputListener>
    {
        public:
            InputDispatcher(const InputData& defaultInput);
            ~InputDispatcher() = default;

            void ProcessInput(const InputData& input);

        private:
            void SendButtonPressEvent(DpadButton button);
            void SendButtonReleasedEvent(DpadButton button);
            void SendJoystickMovedEvent(const JoystickInput& input);
            
            InputData m_LastInput;
    };
}

#endif