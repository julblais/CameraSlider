#include "menu.h"
#include "lcd.h"
#include "joystick.h"

Rig::Menu::Menu(Hardware::LCD &lcd, Hardware::Joystick joystick)
  :m_LCD(lcd), m_Joystick(joystick)
{
}
