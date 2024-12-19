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

    //enum class DpadState : char
    //{
    //    Released = 0,
    //    Pressed = 1
    //};

    constexpr auto DpadNone     = DpadButton::None;
    constexpr auto DpadSelect   = DpadButton::Select;
    constexpr auto DpadUp       = DpadButton::Up;
    constexpr auto DpadDown     = DpadButton::Down;
    constexpr auto DpadLeft     = DpadButton::Left;
    constexpr auto DpadRight    = DpadButton::Right;
    //constexpr auto DpadRelease  = DpadState::Released;
    //constexpr auto DpadPressed  = DpadState::Pressed;

    struct DpadInput
    {
        DpadInput() : button(DpadNone) {}
        DpadInput(DpadButton btn) : button(btn) {}

        DpadButton button;
        //DpadState state;

        //inline bool IsDown() const { return button == DpadDown && state == DpadPressed; }
        //inline bool IsUp() const { return button == DpadUp && state == DpadPressed; }
        //inline bool IsLeft() const { return button == DpadLeft && state == DpadPressed; }
        //inline bool IsRight() const { return button == DpadRight && state == DpadPressed; }
        //inline bool IsSelect() const { return button == DpadSelect && state == DpadPressed; }
        //inline bool IsDefault() const { return button == DpadNone && state == DpadRelease; }

        inline bool IsDown() const { return button == DpadDown; }
        inline bool IsUp() const { return button == DpadUp; }
        inline bool IsLeft() const { return button == DpadLeft; }
        inline bool IsRight() const { return button == DpadRight; }
        inline bool IsSelect() const { return button == DpadSelect; }
        inline bool IsDefault() const { return button == DpadNone; }

        static const char* ToString(DpadButton button);
        //static const char* ToString(DpadState state);
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

        InputData() 
        : dpad(DpadNone), joystick({0, 0}) {}
        InputData(const DpadInput& dpad) :
            dpad(dpad), joystick({0, 0}) {} 
        InputData(const JoystickInput& joystick) :
            dpad(DpadNone), joystick(joystick) {} 
        InputData(const DpadInput& dpad, const JoystickInput& joystick) 
        : dpad(dpad), joystick(joystick) {}
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
            virtual bool OnButtonPressed(const DpadButton button) { return false; }
            virtual bool OnButtonReleased(const DpadButton button) { return false; }
    };
}

#endif