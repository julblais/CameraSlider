#ifndef INPUTREADER_H
#define INPUTREADER_H

#include "input.h"

namespace Input
{
    template<typename TInput>
    class IInputReader
    {
    public:
        virtual ~IInputReader() = default;
        virtual void Setup() {}
        virtual TInput ReadInput() = 0;
    };
}

#endif