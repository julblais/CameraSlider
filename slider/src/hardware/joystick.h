#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

namespace Hardware
{
    class Joystick : public Input::IJoystickReader
    {
        public:
            Joystick(int horizontalPin, int verticalPin, int centerPin);
            virtual ~Joystick() override = default;

            virtual void Init() override;
            virtual Input::JoystickInput ReadInput() override;

        private:
            const int m_HorizontalPin;
            const int m_VerticalPin;
            const int m_CenterPin;
    };
}

#endif