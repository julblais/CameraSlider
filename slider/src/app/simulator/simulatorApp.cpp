#ifdef IS_SIMULATOR

#include "simulatorApp.h"
#include "src/hardware/lcd.h"
#include "src/hardware/deviceInputReader.h"
#include "src/core/perf/perf.h"

#include "src/app/brain/brainApp.h"
#include "src/components/brainConnector.h"

#include "src/commands/settingCommand.h"
#include "src/commands/addressCommand.h"
#include "src/commands/connectionCommand.h"

using namespace Input;
using namespace Output;

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
{
#if defined(IS_BRAIN)
    m_BaseApp = std::unique_ptr<Core::AppBase>(new BrainApp(config));
#elif defined(IS_CONTROLLER)
    m_BaseApp = std::unique_ptr<Core::AppBase>(new ControllerApp(config));
#endif
}

void Slider::SimulatorApp::AddDeviceMessageSimulator() const
{
#if defined(IS_BRAIN)
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest>(
        "connection-sim", [this](ConnectionRequest msg)
        {
            MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
            ConnectionRequest connectMsg(otherMac);
            WifiModule::GetInstance().SimulateSend(msg);
        });
    WifiModule::GetInstance().RegisterSimulateSendCallback<Handshake>("handshake-sim", [](Handshake)
    {
        Handshake handshake(MacAddress({0x11, 0x22, 0x33, 0x44, 0x55, 0x66}));
        WifiModule::GetInstance().SimulateSend(handshake);
    });
#elif defined(IS_CONTROLLER)
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest>(
        "connection-sim", [this](ConnectionRequest msg)
        {
            MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
            Handshake hand(otherMac);
            WifiModule::GetInstance().SimulateSend(hand);
        });
    Timer::FireAndForget("Controller sim timer", 2000, []()
    {
        //Simulate a connection from brain
        MacAddress receiverMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
        ConnectionRequest msg(receiverMac);
        WifiModule::GetInstance().SimulateSend(msg);
    });
#endif
}


void Slider::SimulatorApp::Setup()
{
#if defined(IS_BRAIN)
    auto app = reinterpret_cast<BrainApp*>(m_BaseApp.get());
#elif defined(IS_CONTROLLER)
    auto app = reinterpret_cast<ControllerApp*>(m_BaseApp.get());
#endif

    app->Setup();
    app->m_Display.reset(new Hardware::LCD(m_Config.LcdAddress));
    app->m_DisplayBuffer.Init(app->m_Display.get());

    app->m_InputDispatcher.AddListener([app](const Event& event)
    {
        return OnInputEvent(app->m_DisplayBuffer, event);
    });
    AddDeviceMessageSimulator();
}

void Slider::SimulatorApp::Update()
{
    m_BaseApp->Update();
}

#endif
