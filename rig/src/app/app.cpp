#include "app.h"
#include "hardware/dpad.h"
#include "hardware/lcd.h"
#include "simulator/dpadSimulator.h"

#include <esp32-hal-timer.h>

Rig::App::App(const AppConfig &config):
    m_Config(config)
{
    m_LCD = std::unique_ptr<Hardware::LCD>(new Hardware::LCD(config.LcdAddress));

    if (config.isSimulator)
    {
        m_Dpad = std::unique_ptr<Hardware::IDpad>(new Hardware::DpadSimulator(
            config.DpadVerticalPin, config.DpadHorizontalPin, config.DpadSelectionPin)); 
    }
    
    m_Menu = std::unique_ptr<Menu>(new Menu(m_LCD.get(), m_Dpad.get()));
}

void Rig::App::Setup()
{
    m_LCD->Init();
    m_Dpad->Init();
    m_Menu->Init();

    m_LCD->PrintLn("Salut Guillaume!", 0);
}

void Rig::App::Update()
{
    unsigned long appTimeMs = millis();
    Utils::Timer::Update(appTimeMs);

    m_Dpad->Update();
}