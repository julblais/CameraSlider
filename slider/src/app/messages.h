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

        Type type;
        std::array<char, TEXT_SIZE> text;
    };

    struct ControllerMessage
    {
        enum class Type
        {
            None,
            InputEvent
        };

        Type type;
        Input::Event event;
    };
}


#endif