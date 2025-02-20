#include "app.h"
#include "src/hardware/lcd.h"
#include "src/hardware/dpad.h"
#include "src/hardware/joystick.h"
#include "src/core/perf/perf.h"

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

Slider::App::App(const AppConfig& config) :
    m_Config(config)
{}

void Slider::App::Setup()
{
    m_Display = std::unique_ptr<Display>(new Hardware::LCD(m_Config.LcdAddress));
    auto timer = AddComponent<TimerComponent>();
    auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);

    auto dpad = new Hardware::Dpad(
        m_Config.DpadUpPin,
        m_Config.DpadDownPin,
        m_Config.DpadLeftPin,
        m_Config.DpadRightPin,
        m_Config.DpadSelectionPin);

    m_Dpad = std::unique_ptr<IDpadReader>(dpad);

    m_Joystick = std::unique_ptr<IJoystickReader>(new Hardware::Joystick(
        m_Config.JoystickXPin,
        m_Config.JoystickYPin,
        m_Config.JoystickCenterPin));

    m_InputDispatcher.AddListener(menu);
    m_InputDispatcher.AddListener(stepper);
    m_InputDispatcher.AddListener([this](const Event& event) {
        return OnInputEvent(m_DisplayBuffer, event);
    });

    SetupComponents();
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_Dpad->Init();

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::App::Update()
{
    auto input = InputData(m_Dpad->ReadInput(), m_Joystick->ReadInput());
    /*-> process received messages here <- */
    
    TAKE_SAMPLE(Performance::CpuSampler, "InputDispatch",
    {
        m_InputDispatcher.ProcessInput(input);
    });

    //update all systems
    TAKE_SAMPLE(Performance::CpuSampler, "ComponentUpdate",
    {
        UpdateComponents();
    });

    //output final display buffer
    m_DisplayBuffer.PrintToDisplay();
}
