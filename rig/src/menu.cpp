#include "menu.h"
#include "lcd.h"
#include "dpad.h"
#include <HardwareSerial.h>

Rig::Menu::Menu(Hardware::LCD &lcd, Hardware::Dpad* joystick)
    :m_LCD(lcd), m_Joystick(joystick)
{
}

void Rig::Menu::Init()
{
    Serial.println("adding listener");
    m_Joystick->AddListener(this);
}

void Rig::Menu::OnKeyUp(const Hardware::DpadButton button)
{
    if (button == Hardware::DpadButton::SELECTION)
    {
      Serial.println("here");
        m_LCD.Clear();
        m_LCD.Print("Menu depressed", 0);
    }
}

void Rig::Menu::OnKeyDown(const Hardware::DpadButton button)
{
    if (button == Hardware::DpadButton::SELECTION)
    {
      Serial.println("test! \n");
        m_LCD.Clear();
        m_LCD.Print("Menu pressed", 0);
    }
}
