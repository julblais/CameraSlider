#ifndef DEVICEINPUTREADER_H
#define DEVICEINPUTREADER_H

#include "src/input/input.h"

using namespace Input;

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

    class DeviceInputReader : public InputReader
    {
    public:
        DeviceInputReader(const InputPins& pins);
        ~DeviceInputReader() override;
        void Setup() override;
        InputData ReadInput() override;

    private:
        const InputPins m_Pins;
    };
}

#endif
