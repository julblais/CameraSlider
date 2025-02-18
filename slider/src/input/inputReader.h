#ifndef INPUTREADER_H
#define INPUTREADER_H

#include "input.h"

namespace Input
{
    class InputReader
    {
    public:
        virtual ~InputReader() = default;
        virtual void Setup() {}
        virtual InputData2 ReadInput() = 0;
    };
}

#endif