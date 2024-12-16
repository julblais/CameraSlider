#include <LiquidCrystal_I2C.h>
#include <lcd.h>

#include "debug.h"

Hardware::LCD::LCD(const uint8_t address)
    :chip(address, LCD::NUM_COLS, LCD::NUM_ROWS) 
{}

void Hardware::LCD::Init()
{
    Debug.Log("Init LCD.");
    chip.init();
    chip.backlight();
}

void Hardware::LCD::Clear()
{
    chip.clear();
}