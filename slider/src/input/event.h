#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>

namespace Input
{
    enum class ButtonState
    {
        None,
        Released,
        Pressed
    };

    constexpr auto ButtonNone = ButtonState::None;
    constexpr auto ButtonPressed = ButtonState::Pressed;
    constexpr auto ButtonReleased = ButtonState::Released;

    static const char* ToString(ButtonState button);
    
    struct Event
    {
        bool operator==(const Event& other) const;
        bool operator!=(const Event& other) const;
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
        
        static const char* ToString(const Event& event);
        
        DpadButton button;
        ButtonState dpadButtonState;
        JoystickButton joystickButton;
        ButtonState joystickButtonState;
        float joystickX;
        float joystickY;
        bool joystickDirectionChanged;
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
        Event m_LastEvent;
        unsigned int m_AggregateCount;
    };
}

#endif