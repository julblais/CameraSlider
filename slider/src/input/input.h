#ifndef INPUT_H
#define INPUT_H

#include "Print.h"

namespace Input
{
    enum class DpadButton 
    {
        None,
        Select,
        Up,
        Down,
        Left,
        Right
    };

    enum class JoystickButton
    {
        None,
        Center
    };

    enum class ButtonState
    {
        None,
        Released,
        Pressed
    };

    constexpr auto DpadNone = DpadButton::None;
    constexpr auto DpadSelect = DpadButton::Select;
    constexpr auto DpadUp = DpadButton::Up;
    constexpr auto DpadDown = DpadButton::Down;
    constexpr auto DpadLeft = DpadButton::Left;
    constexpr auto DpadRight = DpadButton::Right;
    constexpr auto JoystickNone = JoystickButton::None;
    constexpr auto JoystickCenter = JoystickButton::Center;
    constexpr auto ButtonNone = ButtonState::None;
    constexpr auto ButtonPressed = ButtonState::Pressed;
    constexpr auto ButtonReleased = ButtonState::Released;

    static const char* ToString(DpadButton button);
    static const char* ToString(JoystickButton button);

    struct InputData2
    {
        InputData2() = default;
        InputData2(DpadButton button, JoystickButton joystickButton, float x, float y);
        DpadButton button;
        float x;
        float y;
        JoystickButton joystickButton;

        inline bool DpadActive() const { return button != DpadNone; }
        inline bool IsDown() const { return button == DpadDown; }
        inline bool IsUp() const { return button == DpadUp; }
        inline bool IsLeft() const { return button == DpadLeft; }
        inline bool IsRight() const { return button == DpadRight; }
        inline bool IsSelect() const { return button == DpadSelect; }
        inline bool IsCenterButton() const { return joystickButton == JoystickCenter; }
    };
    
    class InputReader
    {
    public:
        virtual ~InputReader() = default;
        virtual void Setup() {}
        virtual InputData2 ReadInput() = 0;
    };
}

#endif