#include "stepper.h"

#define DIR_PIN 32
#define STEP_PIN 33
#define MAX_SPEED 1000
#define MAX_ACCEL 100

Stepper::Stepper():
    m_Stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN)
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
