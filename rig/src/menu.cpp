#include "menu.h"
#include "lcd.h"
#include "dpad.h"

Rig::Menu::Menu(Hardware::LCD &lcd, Hardware::Dpad joystick)
  :m_LCD(lcd), m_Joystick(joystick)
{
}
