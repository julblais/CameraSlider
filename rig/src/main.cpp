#include "menu.h"
#include "lcd.h"
#include "dpad.h"
#include "debug.h"

#define VERT_PIN A18
#define HORZ_PIN A19
#define SEL_PIN T8
#define LCD_ADDRESS 0x27

static Hardware::LCD s_lcd(LCD_ADDRESS);
static Hardware::Dpad s_Dpad(VERT_PIN, HORZ_PIN, SEL_PIN);
static Rig::Menu menu(&s_lcd, &s_Dpad);

void setup() {
    Debug.Init(9600);
    Debug.Log("Setup");

    s_Dpad.Init();
    s_lcd.Init();
    menu.Init();

    s_lcd.Print("Salut", 0);
    s_lcd.Print("Guillaume", 1);

    Debug.Log("End setup");
}

void loop() {
    s_Dpad.Update();
}