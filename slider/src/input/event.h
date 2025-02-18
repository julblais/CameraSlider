#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "input.h"
#include "src/core/event/eventSource.h"
#include <functional>
#include "src/core/utils/enumUtils.h"

namespace Input
{
    enum class ButtonChange
    {
        None,
        Released,
        Pressed
    };

    constexpr auto ButtonNone = ButtonChange::None;
    constexpr auto ButtonPressed = ButtonChange::Pressed;
    constexpr auto ButtonReleased = ButtonChange::Released;

    enum class DpadEvent
    {
        None,
        Up,
        Down,
        Left,
        Right,
        Select,
    };

    enum class StickEvent
    {
        None,
        Center
    };

    constexpr auto DpadNone = DpadEvent::None;
    constexpr auto DpadUp = DpadEvent::Up;
    constexpr auto DpadDown = DpadEvent::Down;
    constexpr auto DpadLeft = DpadEvent::Left;
    constexpr auto DpadRight = DpadEvent::Right;
    constexpr auto DpadSelect = DpadEvent::Select;
    constexpr auto StickNone = StickEvent::None;
    constexpr auto StickCenter = StickEvent::Center;

    static const char* ToString(ButtonChange button);
    static const char* ToString(DpadEvent button);
    static const char* ToString(StickEvent button);

    struct EventDiff
    {
        DpadEvent dpad;
        ButtonChange dpadState;
        StickEvent stick;
        ButtonChange joystickState;
        bool joystickMoved;
    };

    struct Event
    {
        Event() = default;
        Event(const Event& previous, const InputData& input);
        bool operator==(const Event& other) const;
        bool operator!=(const Event& other) const;

        bool HasChange() const;
        bool HadDpadChange() const;
        bool HasJoystickChange() const;

        inline DpadEvent GetDpadEvent() const { return dpadEvent; }
        inline StickEvent GetStickEvent() const { return stickEvent; }
        inline bool Is(DpadEvent evt) { return evt == dpadEvent; }
        inline bool Is(StickEvent evt) { return evt == stickEvent; }
        inline bool IsDpadUp() const { return dpadEvent == DpadUp; }
        inline bool IsDpadDown() const { return dpadEvent == DpadDown; }
        inline bool IsDpadLeft() const { return dpadEvent == DpadLeft; }
        inline bool IsDpadRight() const { return dpadEvent == DpadRight; }
        inline bool IsDpadSelect() const { return dpadEvent == DpadSelect; }
        inline bool IsStickCenter() const { return stickEvent == StickCenter; }

        inline float GetStickX() const { return joystickX; }
        inline float GetStickY() const { return joystickY; }
        inline bool HasStickMoved() const { return diff.joystickMoved; }

        inline ButtonChange GetDpadChange() const { return diff.dpadState; }
        inline ButtonChange GetStickChange() const { return diff.joystickState; }
        inline ButtonChange GetChange(DpadEvent evt) const { return dpadEvent == evt ? diff.dpadState : ButtonNone; }
        inline ButtonChange GetChange(StickEvent evt) const { return stickEvent == evt ? diff.joystickState : ButtonNone; }

        static const char* ToString(const Event& event);

    private:
        DpadEvent dpadEvent;
        StickEvent stickEvent;
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