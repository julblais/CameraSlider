#include <LiquidCrystal_I2C.h>
#include <lcd.h>
#include <HardwareSerial.h>

Hardware::LCD::LCD(const uint8_t address)
  :chip(address, LCD::NUM_COLS, LCD::NUM_ROWS) 
{}

void Hardware::LCD::Init()
{
  chip.init();
  chip.backlight();
}

void Hardware::LCD::Clear()
{
  chip.clear();
}