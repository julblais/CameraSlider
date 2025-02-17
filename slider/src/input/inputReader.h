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

    class IDpadReader
    {
    public:
        IDpadReader() = default;
        virtual ~IDpadReader() = default;
        virtual void Init() {}
        virtual DpadInput ReadInput() = 0;
    };

    class IJoystickReader
    {
    public:
        IJoystickReader() = default;
        virtual ~IJoystickReader() = default;
        virtual void Init() {}
        virtual JoystickInput ReadInput() = 0;
    };
}

#endif