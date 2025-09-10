#include "sliderApp.h"
#include "core/serialDisplay.h"
#include "deviceInputReader.h"
#include "core/perf.h"
#include "menu.h"
#include "stepper.h"
#include "bluetoothComponent.h"
#include "settingCommand.h"

using namespace IO;

Slider::SliderApp::SliderApp(const AppConfig& config) :
    m_Config(config) {}

void Slider::SliderApp::Setup()
{
    m_Display = std::unique_ptr<Display>(new SerialDisplay());
    m_DisplayBuffer.Init(m_Display.get());

    AddComponent<TimerComponent>();

    const auto bluetooth = AddComponent<Bt::BluetoothComponent>();
    m_Gamepad = bluetooth->GetGamepad();

    const auto menu = AddComponent<Menu>(&m_DisplayBuffer, m_Config.ShowMenuDelayMs);
    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());
    m_InputDispatcher.AddListener(menu);

    const auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);
    m_InputDispatcher.AddListener(stepper);

    Hardware::InputPins pins{
        .dpadUp = m_Config.DpadUpPin,
        .dpadDown = m_Config.DpadDownPin,
        .dpadLeft = m_Config.DpadLeftPin,
        .dpadRight = m_Config.DpadRightPin,
        .dpadSelection = m_Config.DpadSelectionPin,
        .joystickHorizontal = m_Config.JoystickXPin,
        .joystickVertical = m_Config.JoystickYPin,
        .joystickCenter = m_Config.JoystickCenterPin,
    };
    m_LocalInputReader = std::unique_ptr<InputReader>(new Hardware::DeviceInputReader(pins));
    m_LocalInputReader->Setup();

    SetupComponents();
    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::SliderApp::Update()
{
    TAKE_SAMPLE("ProcessInput", [this]() {
                m_InputDispatcher.ProcessInput(m_LocalInputReader->ReadInput());
                m_InputDispatcher.ProcessInput(m_Gamepad->ReadInput());
                m_InputDispatcher.Dispatch();
                }, CpuSampler);

    //update all systems
    TAKE_SAMPLE("ComponentUpdate", [this]() {
                UpdateComponents();
                }, CpuSampler);

    //output final display buffer
    TAKE_SAMPLE("OutputToLCD", [this]() {
                m_DisplayBuffer.PrintToDisplay();
                }, CpuSampler);
}
