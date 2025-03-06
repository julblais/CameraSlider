#ifndef INPUT_H
#define INPUT_H

#include "buttonEvent.h"

namespace Input
{
    struct InputData
    {
        constexpr InputData() : InputData(ButtonEvent::None, 0.0f, 0.0f) {}
        constexpr InputData(ButtonEvent buttons) : InputData(buttons, 0.0f, 0.0f) {}
        constexpr InputData(float x, float y) : InputData(ButtonEvent::None, x, y) {}
        constexpr InputData(ButtonEvent buttons, float x, float y) : button(buttons), x(x), y(y) {}

        inline bool IsActive() const { return button != ButtonEvent::None; }
        inline bool IsDown() const { return button == ButtonEvent::Down; }
        inline bool IsUp() const { return button == ButtonEvent::Up; }
        inline bool IsLeft() const { return button == ButtonEvent::Left; }
        inline bool IsRight() const { return button == ButtonEvent::Right; }
        inline bool IsSelect() const { return button == ButtonEvent::Select; }
        inline bool IsCenterButton() const { return button == ButtonEvent::Center; }

        ButtonEvent button;
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