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

    enum class DpadState : char
    {
        Released = 0,
        Pressed = 1
    };

    constexpr auto DpadNone     = DpadButton::None;
    constexpr auto DpadSelect   = DpadButton::Select;
    constexpr auto DpadUp       = DpadButton::Up;
    constexpr auto DpadDown     = DpadButton::Down;
    constexpr auto DpadLeft     = DpadButton::Left;
    constexpr auto DpadRight    = DpadButton::Right;
    constexpr auto DpadRelease  = DpadState::Released;
    constexpr auto DpadPressed  = DpadState::Pressed;

    struct DpadInput
    {
        DpadButton button;
        DpadState state;

        inline bool IsDown() const { return button == DpadDown && state == DpadPressed; }
        inline bool IsUp() const { return button == DpadUp && state == DpadPressed; }
        inline bool IsLeft() const { return button == DpadLeft && state == DpadPressed; }
        inline bool IsRight() const { return button == DpadRight && state == DpadPressed; }
        inline bool IsSelect() const { return button == DpadSelect && state == DpadPressed; }
        inline bool IsDefault() const { return button == DpadNone && state == DpadRelease; }

        static const char* ToString(DpadButton button);
        static const char* ToString(DpadState state);
    };

    struct JoystickInput
    {
        int x;
        int y;
    };

    struct InputData
    {
        DpadInput dpad;
        JoystickInput joystick;
    };

    class IDpadReader
    {
        public:
            IDpadReader() = default;
            virtual ~IDpadReader() = default;
            virtual void Init() {}
            virtual DpadInput ReadInput() = 0;
    };

    class IInputListener
    {
        public:
            IInputListener() = default;
            virtual ~IInputListener() = default;
            virtual void OnInput(const InputData& input) {}
            virtual void OnButtonPressed(const DpadButton button) {}
            virtual void OnButtonReleased(const DpadButton button) {}
    };
}

#endif