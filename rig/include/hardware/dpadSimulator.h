#ifndef DPAD_SIM_H
#define DPAD_SIM_H

#include "hardware/dpad.h"

namespace Hardware
{
    class DpadSimulator : public IDpad {
        
        struct State {
            public:
                int Vertical;
                int Horizontal;
                bool Selection;

                State(const int vertical, const int horizontal, const int selection);
                bool IsLeft() const;
                bool IsRight() const;
                bool IsUp() const;
                bool IsDown() const;
        };

        public:
            DpadSimulator(const int verticalPin, const int horizontalPin, const int selectionPin);
            virtual ~DpadSimulator() override {}

            void Init() override;
            void Update() override;

            int ReadVertical() const;
            int ReadHorizontal() const;
            bool SelectionPressed() const;

        private:
            State ReadState() const;
            void ProcessButtons(const State& state) const;

            const int m_VerticalPin;
            const int m_HorizontalPin;
            const int m_SelectionPin;
            State m_LastState;
    };
}

#endif