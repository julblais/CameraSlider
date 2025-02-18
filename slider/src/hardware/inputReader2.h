#ifndef INPUTREADER2_H
#define INPUTREADER2_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

namespace Hardware
{
    struct InputPins
    {
        int dpadUp; int dpadDown; int dpadLeft; int dpadRight; int dpadSelection;
        int joystickHorizontal; int joystickVertical; int joystickCenter;
    };

    class InputReader : public Input::InputReader
    {
    public:

        InputReader(const InputPins& pins);
        virtual ~InputReader() override = default;
        virtual void Setup() override;
        virtual Input::InputData2 ReadInput() override;

    private:
        const InputPins m_Pins;
    };
}

#endif