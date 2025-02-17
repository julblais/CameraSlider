#ifndef INPUTREADER2_H
#define INPUTREADER2_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

using namespace Input;

namespace Hardware
{
    struct InputPins
    {
        int dpadUp; int dpadDown; int dpadLeft; int dpadRight; int dpadSelection;
        int joystickHorizontal; int joystickVertical; int joystickCenter;
    };

    class InputReader : public IInputReader<InputData2>
    {
    public:

        InputReader(const InputPins& pins);
        virtual ~InputReader() override = default;
        virtual void Setup() override;
        virtual InputData2 ReadInput() override;

    private:
        const InputPins m_Pins;
    };
}

#endif