#ifndef INPUTREADER_H
#define INPUTREADER_H

#include "input.h"

namespace Input
{
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