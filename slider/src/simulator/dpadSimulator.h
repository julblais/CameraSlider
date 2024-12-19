#ifndef DPAD_SIM_H
#define DPAD_SIM_H

#include "src/hardware/dpad.h"
#include "src/input/input.h"

namespace Simulator
{
    class DpadSimulator : public Input::IDpadReader {

        public:
            DpadSimulator(const int verticalPin, const int horizontalPin, const int selectionPin);
            virtual ~DpadSimulator() override {}

            virtual void Init() override;
            virtual Input::DpadInput ReadInput() override;

        private:
            static bool IsLeft(const int horizontal);
            static bool IsRight(const int horizontal);
            static bool IsUp(const int vertical);
            static bool IsDown(const int vertical);

            const int m_VerticalPin;
            const int m_HorizontalPin;
            const int m_SelectionPin;
    };
}

#endif