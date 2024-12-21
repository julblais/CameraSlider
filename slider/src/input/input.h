#ifndef INPUT_H
#define INPUT_H

namespace Input
{
    enum class DpadButton : char
    {
        None = 0,
        Select = 1,
        Up = 2,
        Down = 4,
        Left = 8,
        Right = 16
    };

    enum class JoystickButton : char
    {
        None = 0,
        Center = 1
    };

    enum class ButtonState : char
    {
        None = 0,
        Released = 1,
        Pressed = 2
    };

    constexpr auto DpadNone         = DpadButton::None;
    constexpr auto DpadSelect       = DpadButton::Select;
    constexpr auto DpadUp           = DpadButton::Up;
    constexpr auto DpadDown         = DpadButton::Down;
    constexpr auto DpadLeft         = DpadButton::Left;
    constexpr auto DpadRight        = DpadButton::Right;
    constexpr auto JoystickNone     = JoystickButton::None;
    constexpr auto JoystickCenter   = JoystickButton::Center;
    constexpr auto ButtonNone       = ButtonState::None;
    constexpr auto ButtonPressed    = ButtonState::Pressed;
    constexpr auto ButtonReleased   = ButtonState::Released;

    static const char* ToString(DpadButton button);
    static const char* ToString(JoystickButton button);

    struct Event
    {
        DpadButton button;
        ButtonState dpadButtonState;

        JoystickButton joystickButton;
        ButtonState joystickButtonState;
        int joystickX;
        int joystickY;
        bool joystickDirectionChanged;

        bool HasChange() const;
        bool HadDpadChange() const;
        bool HasJoystickChange() const;

        inline bool IsButtonPressed() const { return dpadButtonState == ButtonPressed; }
        inline bool IsJoystickPressed() const { return joystickButtonState == ButtonPressed; }
        inline bool IsDown() const { return button == DpadDown; }
        inline bool IsUp() const { return button == DpadUp; }
        inline bool IsLeft() const { return button == DpadLeft; }
        inline bool IsRight() const { return button == DpadRight; }
        inline bool IsSelect() const { return button == DpadSelect; }
        inline bool IsDefault() const { return button == DpadNone; }
        inline bool IsCenter() const { return joystickButton == JoystickCenter; }
    };

    struct DpadInput { 
        DpadButton button; 
        inline bool IsDown() const { return button == DpadDown; }
        inline bool IsUp() const { return button == DpadUp; }
        inline bool IsLeft() const { return button == DpadLeft; }
        inline bool IsRight() const { return button == DpadRight; }
        inline bool IsSelect() const { return button == DpadSelect; }
        inline bool IsDefault() const { return button == DpadNone; }
    };

    struct JoystickInput { 
        int x; 
        int y; 
        JoystickButton button; 
        inline bool IsCenterButton() const { return button == JoystickCenter; }
    };

    struct InputData { 
        DpadInput dpad; 
        JoystickInput joystick;
        
        InputData() : dpad{DpadNone}, joystick{0, 0, JoystickNone} {}
        InputData(DpadInput dpad, JoystickInput joystick) : dpad(dpad), joystick(joystick) {}
    };
}

#endif