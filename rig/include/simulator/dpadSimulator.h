#ifndef DPAD_SIM_H
#define DPAD_SIM_H

#include "hardware/dpad.h"

namespace Hardware
{
    class DpadSimulator : public IDpad {

        public:
            DpadSimulator(const int verticalPin, const int horizontalPin, const int selectionPin);
            virtual ~DpadSimulator() override {}

            void Init() override;
            virtual DpadButton ReadButton() const override;

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