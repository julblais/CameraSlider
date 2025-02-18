#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>

namespace Input
{
    struct Event : public Printable
    {
        virtual size_t printTo(Print& p) const
        {
            auto size = p.printf("Event: button %i, buttonstate %i, Joystick %i state %i x %f, y%f",
                button, dpadButtonState, joystickButton, joystickButtonState, joystickX, joystickY);
            return size;
        };
        ///////add comparer!!!!!!!!!!!!!!

        DpadButton button;
        ButtonState dpadButtonState;

        JoystickButton joystickButton;
        ButtonState joystickButtonState;
        float joystickX;
        float joystickY;
        bool joystickDirectionChanged;

        bool HasChange() const;
        bool HadDpadChange() const;
        bool HasJoystickChange() const;

        inline bool DpadActive() const { return button != DpadNone; }
        inline bool IsJoystickCenter() const { return joystickButton == JoystickCenter; }
        inline bool IsDpadButtonPressed() const { return dpadButtonState == ButtonPressed; }
        inline bool IsJoystickPressed() const { return joystickButtonState == ButtonPressed; }
        inline bool IsDown() const { return button == DpadDown; }
        inline bool IsUp() const { return button == DpadUp; }
        inline bool IsLeft() const { return button == DpadLeft; }
        inline bool IsRight() const { return button == DpadRight; }
        inline bool IsSelect() const { return button == DpadSelect; }
    };

    class EventDispatcher : public Core::EventSource<const Event&>
    {
    public:
        EventDispatcher() = default;
        void ProcessInput(const InputData& input);
        void Dispatch();
    private:
        InputData m_Input;
        InputData m_Last;
        bool m_ShouldAggregate;
    };
}

#endif