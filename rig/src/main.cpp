#include "menu.h"
#include "lcd.h"
#include "joystick.h"

#define VERT_PIN A18
#define HORZ_PIN A19
#define SEL_PIN T8
#define LCD_ADDRESS 0x27

Hardware::LCD s_lcd(LCD_ADDRESS);
Hardware::Joystick s_joystick(VERT_PIN, HORZ_PIN, SEL_PIN);

void setup() {
  Rig::Menu menu(s_lcd, s_joystick);
  //Serial.begin(115200);
  //Serial.println("Hello \n");

  s_joystick.Init();
  s_lcd.Init();

  s_lcd.Print("Salut", 0);
  s_lcd.Print("Guillaume", 1);
}

void loop() {

  if (s_joystick.SelectionPressed())
  {
    s_lcd.Clear();
    s_lcd.Print("Menu pressed", 0);
  }
}