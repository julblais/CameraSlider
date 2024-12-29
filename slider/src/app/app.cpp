#include "app.h"
#include "src/hardware/lcd.h"
#include "src/hardware/dpad.h"
#include "src/hardware/joystick.h"
#include "messages.h"

#include <esp32-hal-timer.h>

using namespace Input;
using namespace Output;

static bool OnInputEvent(DisplayBuffer& display, const Event& event)
{
    if (event.HasJoystickChange())
    {
        display.PrintLine(0, "Joystick ", event.IsJoystickPressed() ? "pressed" : "");
        display.PrintLine(1, "X: ", event.joystickX, " Y: ", event.joystickY);
    }

    return false;
}

Slider::App::App(const AppConfig &config):
    m_Config(config), m_DisplayBuffer()
{
    SetupComponents(config);
    m_Menu = std::unique_ptr<Menu>(new Menu(&m_DisplayBuffer, config.ShowMenuDelayMs));
    
    m_InputDispatcher.AddListener(m_Menu.get());
    m_InputDispatcher.AddListener(m_Stepper.get());
    m_InputDispatcher.AddListener([this](const Event& event) 
    { 
        return OnInputEvent(m_DisplayBuffer, event); 
    });
}

void Slider::App::SetupComponents(const AppConfig &config)
{    
    m_Display = std::unique_ptr<Display>(new Hardware::LCD(config.LcdAddress));

    m_Dpad = std::unique_ptr<IDpadReader>(new Hardware::Dpad(
        config.DpadUpPin, 
        config.DpadDownPin, 
        config.DpadLeftPin, 
        config.DpadRightPin, 
        config.DpadSelectionPin)); 

    m_Joystick = std::unique_ptr<IJoystickReader>(new Hardware::Joystick(
        config.JoystickXPin,
        config.JoystickYPin,
        config.JoystickCenterPin));

    m_Stepper = std::unique_ptr<Stepper>(new Stepper(
        config.StepperDirectionPin, 
        config.StepperStepPin));
}

void Slider::App::Setup()
{
    LogInfo("Setup components...");
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_Dpad->Init();
    m_Menu->Init();
    m_Stepper->Init();
    LogInfo("Components setup complete.");

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::App::Update()
{
    unsigned long appTimeMs = millis();
    Utils::Timer::Update(appTimeMs);

    auto input = InputData(m_Dpad->ReadInput(), m_Joystick->ReadInput());
    /*-> process received messages here <- */
    m_InputDispatcher.ProcessInput(input);

    //update all systems
    m_Stepper->Update();

    //output final display buffer
    m_DisplayBuffer.PrintToDisplay();
}
