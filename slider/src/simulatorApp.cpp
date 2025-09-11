#if IS_SIMULATOR

#include "simulatorApp.h"
#include "core/serialDisplay.h"
#include "deviceInputReader.h"
#include "core/perf.h"
#include "menu.h"
#include "stepper.h"
#include "autoInput.h"
#include "settingCommand.h"

using namespace IO;
using namespace Core;
using namespace Hardware;

Slider::SimulatorApp::SimulatorApp(const AppConfig& config) :
    m_Config(config) {}

void Slider::SimulatorApp::Setup()
{
    m_Display = std::unique_ptr<Display>(new SerialDisplay());
    m_DisplayBuffer.Setup(m_Display.get());

    InputPins pins{
        .dpadUp = m_Config.DpadUpPin,
        .dpadDown = m_Config.DpadDownPin,
        .dpadLeft = m_Config.DpadLeftPin,
        .dpadRight = m_Config.DpadRightPin,
        .dpadSelection = m_Config.DpadSelectionPin,
        .joystickHorizontal = m_Config.JoystickXPin,
        .joystickVertical = m_Config.JoystickYPin,
        .joystickCenter = m_Config.JoystickCenterPin,
    };
    m_LocalInput = std::unique_ptr<DeviceInputReader>(new DeviceInputReader(pins));
    m_LocalInput->Setup();

    m_AutoInput = std::unique_ptr<AutoInput>(new AutoInput(2700, {
                                                               Instruction::DpadSelect(2500),
                                                               Instruction::DpadDown(),
                                                               Instruction::DpadDown(),
                                                               Instruction::DpadLeft(),
                                                               Instruction::DpadSelect(2500),
                                                               Instruction::Joystick(0.5f, 0.4f, 5000)
                                                           }));

    AddComponent<TimerComponent>();
    const auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    const auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);
    SetupComponents();

    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());

    m_InputDispatcher.AddListener(menu);
    m_InputDispatcher.AddListener(stepper);

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::SimulatorApp::Update()
{
    const auto input = m_AutoInput->ReadInput();
    m_InputDispatcher.ProcessInput(input);
    /*-> ProcessInput(message_from_controller) */
    m_InputDispatcher.Dispatch();

    UpdateComponents();

    //output final display buffer
    m_DisplayBuffer.PrintToDisplay();
}

#endif
