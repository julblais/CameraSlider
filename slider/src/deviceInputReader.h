#ifndef DEVICEINPUTREADER_H
#define DEVICEINPUTREADER_H

#include "input.h"

namespace Hardware
{
    struct InputPins
    {
        int dpadUp;
        int dpadDown;
        int dpadLeft;
        int dpadRight;
        int dpadSelection;
        int joystickHorizontal;
        int joystickVertical;
        int joystickCenter;
    };

    class DeviceInputReader
    {
    public:
        DeviceInputReader(const InputPins& pins);
        ~DeviceInputReader();
        void Setup();
        IO::InputData ReadInput();

    private:
        const InputPins m_Pins;
    };
}

#endif
