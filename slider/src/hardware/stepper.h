#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>
#include "src/core/component/component.h"
#include "src/input/event.h"

namespace Slider
{
    class Stepper : public Component
    {
    public:
        Stepper(const int directionPin, const int stepPin);
        void Setup() override;
        void Update() override;
        bool OnInputEvent(const Input::Event& inputEvent);

    private:
        AccelStepper m_Stepper;
    };
}

#endif