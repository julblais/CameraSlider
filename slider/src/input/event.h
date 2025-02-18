#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>
#include "src/core/utils/enumUtils.h"

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

    enum class ButtonEvent
    {
        None = 0,
        DpadUp = 1 << 0,
        DpadDown = 1 << 1,
        DpadLeft = 1 << 2,
        DpadRight = 1 << 3,
        DpadSelect = 1 << 4,
        Dpad = DpadUp|DpadDown|DpadLeft|DpadRight|DpadSelect,
        StickCenter = 1 << 5,
        Any = Dpad|StickCenter
    };

    static const char* ToString(ButtonState button);

    struct EventDiff
    {
        DpadButton button;
        ButtonState dpadState;
        JoystickButton joystickButton;
        ButtonState joystickState;
        bool joystickDirectionChanged;
    };

    struct Event
    {
        Event(const Event& previous, const InputData& input);
        bool operator==(const Event& other) const;
        bool operator!=(const Event& other) const;

        bool HasChange() const;
        bool HadDpadChange() const;
        bool HasJoystickChange() const;

        inline ButtonEvent GetButtonEvent() const { return buttonEvent; }

        inline bool IsDpadUp() const {  return Core::HasValue(buttonEvent, ButtonEvent::DpadUp);  }
        inline bool IsDpadDown() const;
        inline bool IsDpadLeft() const;
        inline bool IsDpadRight() const;
        inline bool IsDpadSelect() const;
        inline bool IsJoystickCenter() const;

        inline bool GetButtonChangeState(ButtonEvent button) const;

        static const char* ToString(const Event& event);

    private:
        ButtonEvent buttonEvent;
        float joystickX;
        float joystickY;
        EventDiff diff;
    };

    class EventDispatcher : public Core::EventSource<const Event&>
    {
    public:
        EventDispatcher() = default;
        void ProcessInput(const InputData& input);
        void Dispatch();
    private:
        InputData m_Input;
        Event m_LastEvent;
        unsigned int m_AggregateCount;
    };
}

#endif