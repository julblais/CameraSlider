#include "app.h"
#include "dpad.h"
#include "lcd.h"

#define VERT_PIN A18
#define HORZ_PIN A19
#define SEL_PIN T8
#define LCD_ADDRESS 0x27

static Hardware::LCD s_lcd(LCD_ADDRESS);
static Hardware::Dpad s_Dpad(VERT_PIN, HORZ_PIN, SEL_PIN);
static Rig::Menu menu(&s_lcd, &s_Dpad);


Rig::App::App(const AppConfig &config):
    m_LCD(config.LcdAddress),
    m_Dpad(config.DpadVerticalPin, config.DpadHorizontalPin, config.DpadSelectionPin),
    m_Menu(&m_LCD, &m_Dpad)
{
}

void Rig::App::Setup()
{    
    m_Dpad.Init();
    m_LCD.Init();
    m_Menu.Init();

    m_LCD.Print("Salut", 0);
    m_LCD.Print("Guillaume", 1);
}

void Rig::App::Update()
{
    m_Dpad.Update();
}
