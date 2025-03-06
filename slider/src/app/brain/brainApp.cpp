#include "brainApp.h"
#include "src/core/output/serialDisplay.h"
#include "src/hardware/deviceInputReader.h"
#include "src/core/perf/perf.h"
#include "src/app/menu.h"
#include "src/hardware/stepper.h"
#include "src/network/wifiComponent.h"
#include "src/components/brainConnector.h"
#include "src/input/autoInput.h"

#include "src/commands/settingCommand.h"
#include "src/commands/addressCommand.h"
#include "src/commands/connectionCommand.h"

using namespace Input;
using namespace Output;
using namespace Net;

Slider::BrainApp::BrainApp(const AppConfig& config) :
    m_Config(config)
{}

void Slider::BrainApp::Setup()
{
    m_Display = std::unique_ptr<Core::Display>(new Core::SerialDisplay());
    auto timer = AddComponent<TimerComponent>();
    auto wifi = AddComponent<WifiComponent>();
    auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);
    auto connector = AddComponent<BrainConnector>();

    Hardware::InputPins pins;
    pins.dpadUp = m_Config.DpadUpPin;
    pins.dpadDown = m_Config.DpadDownPin;
    pins.dpadLeft = m_Config.DpadLeftPin;
    pins.dpadRight = m_Config.DpadRightPin;
    pins.dpadSelection = m_Config.DpadSelectionPin;
    pins.joystickCenter = m_Config.JoystickCenterPin;
    pins.joystickHorizontal = m_Config.JoystickXPin;
    pins.joystickVertical = m_Config.JoystickYPin;

    //m_InputReader = std::unique_ptr<InputReader>(new Hardware::DeviceInputReader(pins));
    m_InputReader = std::unique_ptr<InputReader>(new AutoInput(2700, {
        Instruction::DpadSelect(2500),
        Instruction::DpadDown(),
        Instruction::DpadDown(),
        Instruction::DpadLeft(),
        Instruction::DpadSelect(2500),
        Instruction::Joystick(0.5f, 0.4f, 5000)
    }));

    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());
    menu->AddCommand(new BrainAddressCommand());
    menu->AddCommand(new ControllerAddressCommand());
    menu->AddCommand(new ConnectionCommand(connector));

    m_InputDispatcher.AddListener(menu);
    m_InputDispatcher.AddListener(stepper);

    SetupComponents();
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_InputReader->Setup();

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::BrainApp::Update()
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
