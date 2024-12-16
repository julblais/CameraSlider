#include "app.h"
#include "hardware/dpad.h"
#include "hardware/lcd.h"

Rig::App::App(const AppConfig &config):
    m_Config(config),
    m_LCD(config.LcdAddress)
{
}

void Rig::App::Setup()
{    
    m_LCD.Init();
    m_Dpad.Init(m_Config.DpadVerticalPin, m_Config.DpadHorizontalPin, m_Config.DpadSelectionPin);
    m_Menu.Init(&m_LCD, &m_Dpad);

    m_LCD.Print("Salut", 0);
    m_LCD.Print("Guillaume", 1);
}

void Rig::App::Update()
{
    m_Dpad.Update();
}
