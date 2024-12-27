#ifndef DPAD_H
#define DPAD_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

namespace Hardware
{
    class Dpad : public Input::IDpadReader {

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