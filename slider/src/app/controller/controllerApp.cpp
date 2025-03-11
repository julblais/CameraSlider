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
    m_Config(config), m_ConnectAnim("ControllerAnimProgress", 500, { " ", ".", "..", "..." })
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
    m_DisplayBuffer.Init(m_Display.get());
    m_InputReader->Setup();
}

void Slider::ControllerApp::Update()
{
    TAKE_SAMPLE("ProcessInput", [this]() {
        auto input = m_InputReader->ReadInput();
        m_InputDispatcher.ProcessInput(input);
        /*-> ProcessInput(message_from_controller) */
        m_InputDispatcher.Dispatch();
    }, CpuSampler);

    if (m_Connector->GetState() == Slider::ControllerConnector::State::CONNECTED)
    {
        m_DisplayBuffer.PrintLine(0, "Connexion");
        m_DisplayBuffer.PrintLine(1, "reussie");
    }

    //update all systems
    TAKE_SAMPLE("ComponentUpdate", [this](){ UpdateComponents(); }, CpuSampler);

    if (m_Connector->GetState() != Slider::ControllerConnector::State::CONNECTED)
    {
        m_DisplayBuffer.PrintLine(0, "Attente de");
        m_DisplayBuffer.PrintLine(1, "connexion", m_ConnectAnim);
    }

    //output final display buffer
    TAKE_SAMPLE("OutputToLCD", [this](){ m_DisplayBuffer.PrintToDisplay(); }, CpuSampler);
}
