#include "app.h"
#include "src/hardware/lcd.h"
#include "src/simulator/dpadSimulator.h"

#include <esp32-hal-timer.h>

Slider::App::App(const AppConfig &config):
    m_Config(config),
    m_InputDispatcher(InputData())
{
    SetupComponents(config);
    m_Menu = std::unique_ptr<Menu>(new Menu(m_LCD.get(), config.ShowMenuDelayMs));
    m_InputDispatcher.AddListener(m_Menu.get());
}

void Slider::App::SetupComponents(const AppConfig &config)
{    
    m_LCD = std::unique_ptr<Hardware::LCD>(new Hardware::LCD(config.LcdAddress));

    #ifdef IS_SIMULATOR
        m_Dpad = std::unique_ptr<Input::IDpadReader>(new Simulator::DpadSimulator(
                config.DpadVerticalPin, config.DpadHorizontalPin, config.DpadSelectionPin)); 
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

    auto input = InputData(m_Dpad->ReadInput());
    //process received messages
    m_InputDispatcher.ProcessInput(input);
}
