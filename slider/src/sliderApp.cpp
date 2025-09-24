#include "sliderApp.h"

#if !IS_SIMULATOR

#include "core/serialDisplay.h"
#include "deviceInputReader.h"
#include "core/perf.h"
#include "core/display.h"
#include "core/serialReader.h"
#include "displayBuffer.h"
#include "eventDispatcher.h"
#include "menu.h"
#include "stepper.h"
#include "bluetoothComponent.h"
#include "bluetoothCommands.h"
#include "bluetoothGamepad.h"
#include "settingCommand.h"

using namespace Core;
using namespace IO;
using namespace Hardware;
using namespace Bt;

Slider::SliderApp::SliderApp(const AppConfig& config) :
    m_Config(config) {}

bool Slider::SliderApp::OnInputEvent(const Event& inputEvent)
{
    if (inputEvent.HasChange())
    {
        m_DisplayBuffer->Clear();
        m_DisplayBuffer->Print(inputEvent);
    }
    return false;
}

bool Slider::SliderApp::OnSerialMessage(const char* message)
{
    LogInfo("Received serial message: ", message);
    if (strcmp(message, "bt.pair") == 0)
    {
        if (m_BluetoothComponent != nullptr)
            m_BluetoothComponent->EnablePairing();
        else
            LogError("Cannot reset Bluetooth module: Bluetooth component is null");
        return true;
    }
    return false;
}

void Slider::SliderApp::Setup()
{
    LogDebug("Creating display...");
    m_Display = std::unique_ptr<Display>(new SerialDisplay());
    m_DisplayBuffer = std::unique_ptr<DisplayBuffer>(new DisplayBuffer());
    m_DisplayBuffer->Setup(m_Display.get());

    LogDebug("Creating input...");
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

    LogDebug("Creating components...");
    AddComponent<TimerComponent>();
    m_BluetoothComponent = AddComponent<BluetoothComponent>();
    const auto menu = AddComponent<Menu>(m_DisplayBuffer.get(), m_Config.ShowMenuDelayMs);
    const auto stepper = AddComponent<Stepper>(m_Config.StepperDirectionPin, m_Config.StepperStepPin);
    SetupComponents();

    LogDebug("Creating serial reader");
    m_SerialReader = std::unique_ptr<SerialReader>(new SerialReader());
    m_SerialReader->AddListener(this);

    LogDebug("Creating listeners...");
    m_InputDispatcher = std::unique_ptr<EventDispatcher>(new EventDispatcher());
    m_InputDispatcher->AddListener(menu);
    m_InputDispatcher->AddListener(stepper);
    m_InputDispatcher->AddListener(this);

    LogDebug("Adding menu commands...");
    menu->AddCommand(new MaxSpeedCommand());
    menu->AddCommand(new SpeedCurveCommand());
    menu->AddCommand(new GamepadNameCommand(m_BluetoothComponent));
    menu->AddCommand(new GamepadConnectionCommand(m_BluetoothComponent));

    LogDebug("Printing display buffer...");
    m_DisplayBuffer->Print(Event());
}

void Slider::SliderApp::Update()
{
    TAKE_SAMPLE("Process messages", [this]() {
                m_SerialReader->ProcessInput();
                }, CpuSampler);

    TAKE_SAMPLE("ProcessInput", [this]() {
                //m_InputDispatcher.ProcessInput(m_LocalInput->ReadInput());
                m_InputDispatcher->ProcessInput(m_BluetoothComponent->GetGamepad()->ReadInput());
                m_InputDispatcher->Dispatch();
                }, CpuSampler);

    //update all systems
    TAKE_SAMPLE("ComponentUpdate", [this]() {
                UpdateComponents();
                }, CpuSampler);

    //output final display buffer
    TAKE_SAMPLE("OutputToLCD", [this]() {
                m_DisplayBuffer->PrintToDisplay();
                }, CpuSampler);
}

#endif
