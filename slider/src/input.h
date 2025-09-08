#ifndef INPUT_H
#define INPUT_H

#include "buttonEvent.h"

namespace Input
{
    struct InputData
    {
        constexpr InputData() :
            InputData(ButtonEvent::None, 0.0f, 0.0f) {}

        constexpr InputData(const ButtonEvent buttons) :
            InputData(buttons, 0.0f, 0.0f) {}

        constexpr InputData(const float x, const float y) :
            InputData(ButtonEvent::None, x, y) {}

        constexpr InputData(const ButtonEvent buttons, const float x, const float y) :
            button(buttons),
            x(x),
            y(y) {}

        bool IsActive() const { return button != ButtonEvent::None; }
        bool IsDown() const { return button == ButtonEvent::Down; }
        bool IsUp() const { return button == ButtonEvent::Up; }
        bool IsLeft() const { return button == ButtonEvent::Left; }
        bool IsRight() const { return button == ButtonEvent::Right; }
        bool IsSelect() const { return button == ButtonEvent::Select; }
        bool IsCenterButton() const { return button == ButtonEvent::Center; }

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
