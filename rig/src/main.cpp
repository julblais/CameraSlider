#include "menu.h"
#include "lcd.h"
#include "dpad.h"

#include <HardwareSerial.h>

#define VERT_PIN A18
#define HORZ_PIN A19
#define SEL_PIN T8
#define LCD_ADDRESS 0x27

static Hardware::LCD s_lcd(LCD_ADDRESS);
static Hardware::Dpad s_Dpad(VERT_PIN, HORZ_PIN, SEL_PIN);
static Rig::Menu menu(&s_lcd, &s_Dpad);

void setup() {    
    Serial.begin(9600);
    Serial.println("Hello! \n");

    s_Dpad.Init();
    s_lcd.Init();
    menu.Init();

    s_lcd.Print("Salut", 0);
    s_lcd.Print("Guillaume", 1);
}

void loop() {
    s_Dpad.Update();
}