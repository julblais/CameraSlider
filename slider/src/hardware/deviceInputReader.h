#ifndef DEVICEINPUTREADER_H
#define DEVICEINPUTREADER_H

#include "src/input/input.h"

using namespace Input;

namespace Hardware
{
    struct InputPins
    {
        int dpadUp; int dpadDown; int dpadLeft; int dpadRight; int dpadSelection;
        int joystickHorizontal; int joystickVertical; int joystickCenter;
    };

    class DeviceInputReader : public Input::InputReader
    {
    public:
        DeviceInputReader(const InputPins& pins);
        virtual ~DeviceInputReader() override = default;
        virtual void Setup() override;
        virtual InputData ReadInput() override;

    private:
        const InputPins m_Pins;
    };
}

#endif