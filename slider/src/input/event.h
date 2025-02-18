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

    using namespace Core::Enums;
    enum class DpadEvent
    {
        None = 0,
        Up = 1 << 0,
        Down = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        Select = 1 << 4,
        Any = Up | Down | Left | Right | Select,
    };

    enum class StickEvent
    {
        None = 0,
        Center = 1 << 0,
        MoveX = 1 << 1,
        MoveY = 1 << 2,
        Move = MoveX | MoveY,
        Any = Center | Move
    };

    constexpr auto DpadUp = DpadEvent::Up;
    constexpr auto DpadDown = DpadEvent::Down;
    constexpr auto DpadLeft = DpadEvent::Left;
    constexpr auto DpadRight = DpadEvent::Right;
    constexpr auto DpadSelect = DpadEvent::Select;
    constexpr auto StickMoveX = StickEvent::MoveX;
    constexpr auto StickMoveY = StickEvent::MoveY;
    constexpr auto StickMove = StickEvent::Move;
    constexpr auto StickCenter = StickEvent::Center;
    constexpr auto DpadAny = DpadEvent::Any;
    constexpr auto StickAny = StickEvent::Any;

    static const char* ToString(ButtonState button);

    struct EventDiff
    {
        DpadEvent dpad;
        ButtonState dpadState;
        StickEvent stick;
        ButtonState joystickState;
        bool joystickDirectionChanged;
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
        inline bool Is(DpadEvent evt) { return HasValue(dpadEvent, evt); }
        inline bool Is(StickEvent evt) { return HasValue(stickEvent, evt); }
        inline bool IsDpadUp() const { return HasValue(dpadEvent, DpadUp); }
        inline bool IsDpadDown() const { return HasValue(dpadEvent, DpadDown); }
        inline bool IsDpadLeft() const { return HasValue(dpadEvent, DpadLeft); }
        inline bool IsDpadRight() const { return HasValue(dpadEvent, DpadRight); }
        inline bool IsDpadSelect() const { return HasValue(dpadEvent, DpadSelect); }
        inline bool IsStickCenter() const { return HasValue(stickEvent, StickCenter); }

        inline float GetStickX() const { return joystickX; }
        inline float GetStickY() const { return joystickY; }

        inline ButtonState GetChangeState(DpadEvent evt) const { return HasValue(diff.dpad, evt) ? diff.dpadState : ButtonNone; }
        inline ButtonState GetChangeState(StickEvent evt) const { return HasValue(diff.stick, evt) ? diff.joystickState : ButtonNone; }

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