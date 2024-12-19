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

    enum class ButtonState : char
    {
        None = 0,
        Pressed = 1
    };

    struct DpadInput
    {
        DpadButton button;
        ButtonState state;
    };

    class IDpadReader
    {
        public:
            virtual DpadInput ReadInput() = 0;
            IDpadReader() = default;
            virtual ~IDpadReader() = default;
    };
}

#endif