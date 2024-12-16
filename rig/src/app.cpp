#include "app.h"
#include "hardware/dpad.h"
#include "hardware/dpadSimulator.h"
#include "hardware/lcd.h"

#include <memory>

Rig::App::App(const AppConfig &config):
    m_Config(config)
{
    m_LCD = std::unique_ptr<Hardware::LCD>(new Hardware::LCD(config.LcdAddress));
    m_Menu = std::unique_ptr<Menu>(new Menu());

    if (config.isSimulator)
    {
        m_Dpad = std::unique_ptr<Hardware::IDpad>(new Hardware::DpadSimulator(
            config.DpadVerticalPin, config.DpadHorizontalPin, config.DpadSelectionPin)); 
    }
}

void Rig::App::Setup()
{    
    m_LCD->Init();
    m_Dpad->Init();
    m_Menu->Init(m_LCD.get(), m_Dpad.get());

    m_LCD->Print("Salut", 0);
    m_LCD->Print("Guillaume", 1);
}

void Rig::App::Update()
{
    m_Dpad->Update();
}
