#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>
#include "src/input/input.h"

class Stepper
{
    public:
        Stepper(const unsigned char directionPin, const unsigned char stepPin);
        void Init();
        void Update();
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        AccelStepper m_Stepper;
};

#endif