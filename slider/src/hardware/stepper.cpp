#include "stepper.h"

#define MAX_SPEED 1000
#define MAX_ACCEL 100

Stepper::Stepper(const int directionPin, const int stepPin):
    m_Stepper(AccelStepper::DRIVER, stepPin, directionPin)
{}

void Stepper::Init()
{
    m_Stepper.setMaxSpeed(MAX_SPEED);
    m_Stepper.setAcceleration(MAX_ACCEL);
}

void Stepper::Update()
{
    m_Stepper.runSpeed();
}

bool Stepper::OnInputEvent(const Input::Event &inputEvent)
{
    if (inputEvent.joystickX < 0)
    {
        m_Stepper.setSpeed(-50);
    }
    else if (inputEvent.joystickX > 0)
    {
        m_Stepper.setSpeed(50);
    }
    else
    {
        m_Stepper.setSpeed(0);
    }

    return false;
}
