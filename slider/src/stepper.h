#ifndef STEPPER_H
#define STEPPER_H

#include <AccelStepper.h>
#include "core/component.h"
#include "event.h"

namespace Slider
{
    class Stepper : public Core::Component
    {
    public:
        Stepper(const int directionPin, const int stepPin);
        void Setup() override;
        void Update() override;
        bool OnInputEvent(const IO::Event& inputEvent);

    private:
        AccelStepper m_Stepper;
    };
}

#endif