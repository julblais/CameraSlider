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

    constexpr auto DpadNone = DpadButton::None;
    constexpr auto DpadSelect = DpadButton::Select;
    constexpr auto DpadUp = DpadButton::Up;
    constexpr auto DpadDown = DpadButton::Down;
    constexpr auto DpadLeft = DpadButton::Left;
    constexpr auto DpadRight = DpadButton::Right;
    constexpr auto JoystickNone = JoystickButton::None;
    constexpr auto JoystickCenter = JoystickButton::Center;

    static const char* ToString(DpadButton button);
    static const char* ToString(JoystickButton button);

    struct InputData
    {
        InputData() = default;
        InputData(DpadButton button, JoystickButton joystickButton, float x, float y);

        inline bool DpadActive() const { return dpadButton != DpadNone; }
        inline bool IsDown() const { return dpadButton == DpadDown; }
        inline bool IsUp() const { return dpadButton == DpadUp; }
        inline bool IsLeft() const { return dpadButton == DpadLeft; }
        inline bool IsRight() const { return dpadButton == DpadRight; }
        inline bool IsSelect() const { return dpadButton == DpadSelect; }
        inline bool IsCenterButton() const { return joystickButton == JoystickCenter; }

        DpadButton dpadButton;
        JoystickButton joystickButton;
        float x;
        float y;
    };

    class InputReader
    {
    public:
        virtual ~InputReader() = default;
        virtual void Setup() {}
        virtual InputData ReadInput() = 0;
    };
}

#endif