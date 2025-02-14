#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>
#include "src/component/component.h"
#include "src/input/input.h"

namespace Slider
{
    class Stepper : public Component
    {
    public:
        Stepper(const int directionPin, const int stepPin);
        virtual void Setup() override;
        virtual void Update() override;
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        AccelStepper m_Stepper;
    };
}

#endif