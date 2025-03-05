#include "controllerApp.h"
#include "src/core/output/serialDisplay.h"
#include "src/hardware/deviceInputReader.h"
#include "src/core/perf/perf.h"
#include "src/network/wifiComponent.h"

#include "src/commands/settingCommand.h"
#include "src/commands/addressCommand.h"
#include "src/commands/connectionCommand.h"

using namespace Input;
using namespace Output;
using namespace Net;

Slider::ControllerApp::ControllerApp(const AppConfig& config) :
    m_Config(config)
{}

void Slider::ControllerApp::Setup()
{
    m_Display = std::unique_ptr<Core::Display>(new Core::SerialDisplay());
    auto timer = AddComponent<TimerComponent>();
    auto wifi = AddComponent<WifiComponent>();
    m_Connector = AddComponent<ControllerConnector>();

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

    SetupComponents();
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_InputReader->Setup();

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::ControllerApp::Update()
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

    if (m_Connector->GetState() == Slider::ControllerConnector::State::CONNECTED)
    {
        //output final display buffer
        TAKE_SAMPLE(CpuSampler, "OutputToLCD",
        {
            m_DisplayBuffer.PrintToDisplay();
        });
    }
    else
    {
        m_Display->PrintLine(0, "Attente de");
        m_Display->PrintLine(1, "connexion...");
    }
}
