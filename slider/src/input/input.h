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

    const char* ToString(DpadButton button);
    const char* ToString(JoystickButton button);

    struct InputData
    {
        InputData() = default;
        InputData(DpadButton button, JoystickButton joystickButton, float x, float y);

        inline bool DpadActive() const { return dpadButton != DpadButton::None; }
        inline bool IsDown() const { return dpadButton == DpadButton::Down; }
        inline bool IsUp() const { return dpadButton == DpadButton::Up; }
        inline bool IsLeft() const { return dpadButton == DpadButton::Left; }
        inline bool IsRight() const { return dpadButton == DpadButton::Right; }
        inline bool IsSelect() const { return dpadButton == DpadButton::Select; }
        inline bool IsCenterButton() const { return joystickButton == JoystickButton::Center; }

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