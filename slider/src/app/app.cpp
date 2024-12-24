#include "app.h"
#include "src/hardware/lcd.h"
#include "src/simulator/dpadSimulator.h"
#include "src/simulator/joystickSimulator.h"

#include <esp32-hal-timer.h>

static bool OnInputEvent(Output::DisplayBuffer& display, const Input::Event& event)
{
    if (event.HasJoystickChange())
    {
        display.PrintLine(0, "Joystick ", event.IsJoystickPressed() ? "pressed" : "");
        display.PrintLine(1, "X: ", event.joystickX, " Y: ", event.joystickY);
    }

    return false;
}

Slider::App::App(const AppConfig &config):
    m_Config(config), m_DisplayBuffer()
{
    SetupComponents(config);
    m_Menu = std::unique_ptr<Menu>(new Menu(&m_DisplayBuffer, config.ShowMenuDelayMs));
    
    m_InputDispatcher.AddListener(m_Menu.get());
    m_InputDispatcher.AddListener([this](const Input::Event& event) 
    { 
        return OnInputEvent(m_DisplayBuffer, event); 
    });
}

void Slider::App::SetupComponents(const AppConfig &config)
{    
    m_Display = std::unique_ptr<Hardware::LCD>(new Hardware::LCD(config.LcdAddress));

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
    m_Display->Init();
    m_DisplayBuffer.Init(m_Display.get());
    m_Dpad->Init();
    m_Menu->Init();

    m_DisplayBuffer.PrintLine(0, "Salut Guillaume!");
}

void Slider::App::Update()
{
    unsigned long appTimeMs = millis();
    Utils::Timer::Update(appTimeMs);

    auto input = Input::InputData(m_Dpad->ReadInput(), m_Joystick->ReadInput());
    //process received messages
    m_InputDispatcher.ProcessInput(input);

    //output final display buffer
    m_DisplayBuffer.PrintToDisplay();

    //other idea: for the menu
    //remove isOpened from menu system
    //add an update method to menu
    //add a timer to menu with the menu open delay
    //the update method will print the output for the menu

}
