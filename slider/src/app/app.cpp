#include "app.h"
#include "src/hardware/lcd.h"
#include "src/simulator/dpadSimulator.h"
#include "src/simulator/joystickSimulator.h"

#include <esp32-hal-timer.h>


static bool OnInputEvent(Hardware::LCD* lcd, const Input::Event& event)
{
    if (event.HasJoystickChange())
    {
        lcd->PrintLn("Joystick ", event.IsJoystickPressed() ? "pressed" : "", 0);
        lcd->PrintLn("X: ", event.joystickX, " Y: ", event.joystickY, 1);
    }
    return false;
}

Slider::App::App(const AppConfig &config):
    m_Config(config)
{
    SetupComponents(config);
    m_Menu = std::unique_ptr<Menu>(new Menu(m_LCD.get(), config.ShowMenuDelayMs));
    
    m_InputDispatcher.AddListener(m_Menu.get());
    m_InputDispatcher.AddListener([this](const Input::Event& event) 
    { 
        return OnInputEvent(m_LCD.get(), event); 
    });
}

void Slider::App::SetupComponents(const AppConfig &config)
{    
    m_LCD = std::unique_ptr<Hardware::LCD>(new Hardware::LCD(config.LcdAddress));

    #ifdef IS_SIMULATOR
        m_Dpad = std::unique_ptr<Input::IDpadReader>(new Simulator::DpadSimulator(
        config.DpadUpPin, 
        config.DpadDownPin, 
        config.DpadLeftPin, 
        config.DpadRightPin, 
        config.DpadSelectionPin)); 

        m_Joystick = std::unique_ptr<Input::IJoystickReader>(new Simulator::JoystickSimulator(
        config.JoystickXPin,
        config.JoystickYPin,
        config.JoystickCenterPin));
    #else
        m_Dpad = std::unique_ptr<Input::IDpadReader>(new Simulator::DpadSimulator(
        config.DpadUpPin, 
        config.DpadDownPin, 
        config.DpadLeftPin, 
        config.DpadRightPin, 
        config.DpadSelectionPin)); 

        m_Joystick = std::unique_ptr<Input::IJoystickReader>(new Simulator::JoystickSimulator(
        config.JoystickXPin,
        config.JoystickYPin,
        config.JoystickCenterPin));
    #endif
}

void Slider::App::Setup()
{
    m_LCD->Init();
    m_Dpad->Init();
    m_Menu->Init();

    m_LCD->PrintLn("Salut Guillaume!", 0);
}

void Slider::App::Update()
{
    unsigned long appTimeMs = millis();
    Utils::Timer::Update(appTimeMs);

    auto input = Input::InputData(m_Dpad->ReadInput(), m_Joystick->ReadInput());
    //process received messages
    m_InputDispatcher.ProcessInput(input);
}
