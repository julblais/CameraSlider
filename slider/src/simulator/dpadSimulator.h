#ifndef DPAD_SIM_H
#define DPAD_SIM_H

#include "src/input/input.h"

namespace Simulator
{
    class DpadSimulator : public Input::IDpadReader {

        public:
            DpadSimulator(int upPin, int downPin, int leftPin, int rightPin, int selectionPin);
            virtual ~DpadSimulator() override {}

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