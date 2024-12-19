#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/utils/eventSource.h"

namespace Input
{
    class InputManager : public Utils::EventSource<IInputListener>
    {
        public:
            InputManager(const InputData& defaultInput);
            ~InputManager() = default;

            void ProcessInput(const InputData& input);

        private:
            void SendInputEvent(const InputData& input);
            void SendKeyPressedEvent(DpadButton button);
            void SendKeyReleasedEvent(DpadButton button);
            
            InputData m_LastInput;
    };
}

#endif