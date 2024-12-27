#ifndef DPAD_H
#define DPAD_H

#include "src/input/input.h"
#include "src/input/inputReader.h"

namespace Hardware
{
    class Dpad : public Input::IDpadReader {

        public:
            Dpad(
                const unsigned char upPin, const unsigned char downPin, 
                const unsigned char leftPin, const unsigned char rightPin, 
                const unsigned char selectionPin);
            virtual ~Dpad() override = default;

            virtual void Init() override;
            virtual Input::DpadInput ReadInput() override;

        private:
            const unsigned char m_UpPin;
            const unsigned char m_DownPin;
            const unsigned char m_LeftPin;
            const unsigned char m_RightPin;
            const unsigned char m_SelectionPin;
    };
}

#endif