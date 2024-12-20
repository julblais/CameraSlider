#ifndef JOYSTICKSIMULATOR_H
#define JOYSTICKSIMULATOR_H

#include "src/input/input.h"

namespace Simulator
{
    class JoystickSimulator : public Input::IJoystickReader
    {
        public:
            JoystickSimulator(int horizontalPin, int verticalPin, int centerPin);
            virtual ~JoystickSimulator() override = default;

            virtual void Init() override;
            virtual Input::JoystickInput ReadInput() override;

        private:
            const int m_HorizontalPin;
            const int m_VerticalPin;
            const int m_CenterPin;
    };
}

#endif