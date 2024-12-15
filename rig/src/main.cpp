#include "menu.h"
#include "lcd.h"

#define VERT_PIN A18
#define HORZ_PIN A19
#define SEL_PIN T8
#define LCD_ADDRESS 0x27

Utils::LCD LCD(LCD_ADDRESS);

void setup() {
  Rig::Menu menu(LCD);
  //Serial.begin(115200);
  //Serial.println("Hello \n");
  
  pinMode(VERT_PIN, INPUT);
  pinMode(HORZ_PIN, INPUT);
  pinMode(SEL_PIN, INPUT_PULLUP);

  LCD.Init();
  LCD.Print("Salut", 0);
  LCD.Print("Guillaume", 1);
}

void loop() {
  // horz goes from 0 (right) to 1023 (left)
  int vert = analogRead(VERT_PIN);
  // vert goes from 0 (bottom) to 1023 (top)
  int horz = analogRead(HORZ_PIN);
  // selPressed is true is the joystick is pressed
  bool selPressed = digitalRead(SEL_PIN) == LOW;

  if (selPressed)
  {
    LCD.Clear();
    LCD.Print("Menu pressed", 0);
  }
}