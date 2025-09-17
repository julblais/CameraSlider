#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

namespace IO
{
    enum class ButtonEvent
    {
        None,
        Up,
        Down,
        Left,
        Right,
        Select,
        Center
    };

    enum class ButtonChange
    {
        None = 0,
        Released = 1 << 0,
        Pressed = 1 << 1
    };

    constexpr auto None = ButtonEvent::None;
    constexpr auto DpadUp = ButtonEvent::Up;
    constexpr auto DpadDown = ButtonEvent::Down;
    constexpr auto DpadLeft = ButtonEvent::Left;
    constexpr auto DpadRight = ButtonEvent::Right;
    constexpr auto DpadSelect = ButtonEvent::Select;
    constexpr auto StickCenter = ButtonEvent::Center;

    constexpr auto ButtonNone = ButtonChange::None;
    constexpr auto ButtonPressed = ButtonChange::Pressed;
    constexpr auto ButtonReleased = ButtonChange::Released;

    const char* ToString(const ButtonEvent button);
    const char* ToString(const ButtonChange button);
}
#endif