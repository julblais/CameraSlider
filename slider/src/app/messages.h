#ifndef MESSAGES_H
#define MESSAGES_H

#include "src/output/displayBuffer.h"
#include "src/input/input.h"

#include <array>

namespace Messages
{
    struct BrainMessage
    {
        constexpr static auto TEXT_SIZE = Output::LCD_LINE_LENGTH * Output::LCD_NUM_LINES;

        enum class Type
        {
            None,
            DisplayText
        };

        static BrainMessage BuildDisplayTextMessage(const std::array<char, TEXT_SIZE>& text);

        inline bool IsDisplayText() const { return type == Type::DisplayText; }

        const Type type;
        std::array<char, TEXT_SIZE> text;
    };

    struct ControllerMessage
    {
        enum class Type
        {
            None,
            InputEvent
        };

        static ControllerMessage BuildDisplayTextMessage(const Input::Event& event);

        inline bool IsInputEvent() const { return type == Type::InputEvent; }

        const Type type;
        Input::Event event;
    };
}


#endif