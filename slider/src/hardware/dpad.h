#ifndef DPAD_H
#define DPAD_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

using namespace Input;

namespace Hardware
{
    class InputReader : public IInputReader<InputData2>
    {
    public:
        struct Pins
        {
            int dpadUp; int dpadDown; int dpadLeft; int dpadRight; int dpadSelection;
            int joystickHorizontal; int joystickVertical; int joystickCenter;
        };

        InputReader(const Pins& pins);
        virtual ~InputReader() override = default;
        virtual void Setup() override {}
        virtual InputData2 ReadInput() override;

    private:
        const Pins m_Pins;
    };

    class Dpad : public Input::IDpadReader
    {

    public:
        Dpad(int upPin, int downPin, int leftPin, int rightPin, int selectionPin);
        virtual ~Dpad() override = default;

        virtual void Init() override;
        virtual Input::DpadInput ReadInput() override;

    private:
        const int m_UpPin;
        const int m_DownPin;
        const int m_LeftPin;
        const int m_RightPin;
        const int m_SelectionPin;
    };
}

#endif