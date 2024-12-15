#include <LiquidCrystal_I2C.h>
#include <lcd.h>

Utils::LCD::LCD(const uint8_t address)
  :chip(address, LCD::NUM_COLS, LCD::NUM_ROWS) 
{}

void Utils::LCD::Init()
{
  chip.init();
  chip.backlight();
}

void Utils::LCD::Clear()
{
  chip.clear();
}