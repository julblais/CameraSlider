#include "simulatorApp.h"
#include "src/hardware/lcd.h"
#include "src/hardware/deviceInputReader.h"
#include "src/core/perf/perf.h"
#include "src/network/wifiComponent.h"

#include "src/commands/settingCommand.h"
#include "src/commands/addressCommand.h"
#include "src/commands/connectionCommand.h"

using namespace Input;
using namespace Output;
using namespace Net;

static bool OnInputEvent(DisplayBuffer& display, const Event& event)
{
    if (event.HasChange())
    {
        display.PrintLine(0, "Joystick ", event.IsStickCenter() ? "pressed" : "");
        display.PrintLine(1, "X: ", event.GetStickX(), " Y: ", event.GetStickY());
    }

    return false;
}

Slider::SimulatorApp::SimulatorApp(const AppConfig& config) :
    m_Config(config)
{}

void Slider::SimulatorApp::Setup()
{
    m_Display = std::unique_ptr<Core::Display>(new Hardware::LCD(m_Config.LcdAddress));
    auto timer = AddComponent<TimerComponent>();
    auto wifi = AddComponent<WifiComponent>();
    auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);

    Hardware::InputPins pins;
    pins.dpadUp = m_Config.DpadUpPin;
    pins.dpadDown = m_Config.DpadDownPin;
    pins.dpadLeft = m_Config.DpadLeftPin;
    pins.dpadRight = m_Config.DpadRightPin;
    pins.dpadSelection = m_Config.DpadSelectionPin;
    pins.joystickCenter = m_Config.JoystickCenterPin;
    pins.joystickHorizontal = m_Config.JoystickXPin;
    pins.joystickVertical = m_Config.JoystickYPin;

    m_InputReader = std::unique_ptr<Input::InputReader>(new Hardware::DeviceInputReader(pins));

    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());
    menu->AddCommand(new BrainAddressCommand());
    menu->AddCommand(new ControllerAddressCommand());
    menu->AddCommand(new ConnectionCommand());

    m_InputDispatcher.AddListener(menu);
    m_InputDispatcher.AddListener(stepper);
    m_InputDispatcher.AddListener([this](const Event& event) {
        return OnInputEvent(m_DisplayBuffer, event);
    });

    SetupComponents();
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_InputReader->Setup();

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::SimulatorApp::Update()
{
    TAKE_SAMPLE(CpuSampler, "ProcessInput",
    {
        auto input = m_InputReader->ReadInput();
        m_InputDispatcher.ProcessInput(input);
        /*-> ProcessInput(message_from_controller) */
        m_InputDispatcher.Dispatch();
    });

    //update all systems
    TAKE_SAMPLE(CpuSampler, "ComponentUpdate",
    {
        UpdateComponents();
    });

    //output final display buffer
    TAKE_SAMPLE(CpuSampler, "OutputToLCD",
    {
        m_DisplayBuffer.PrintToDisplay();
    });
}
