#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

namespace Hardware
{
    class Joystick : public Input::IJoystickReader
    {
        public:
            Joystick(const unsigned char horizontalPin, const unsigned char verticalPin, const unsigned char centerPin);
            virtual ~Joystick() override = default;

            virtual void Init() override;
            virtual Input::JoystickInput ReadInput() override;

        private:
            const unsigned char m_HorizontalPin;
            const unsigned char m_VerticalPin;
            const unsigned char m_CenterPin;
    };
}

#endif