
#include <hardware/lcd.h>

#include <LiquidCrystal_I2C.h>
#include <array>
#include <algorithm>

#include "debug.h"

std::array<char, Hardware::LCD::NUM_COLS> m_Buffer;

Hardware::LCD::LCD(const uint8_t address):
    chip(address, LCD::NUM_COLS, LCD::NUM_ROWS)
{
}

void Hardware::LCD::Init()
{
    m_Buffer.fill(' ');

    Debug.Log("Init LCD.");
    chip.init();
    chip.backlight();
}

void Hardware::LCD::Print(const char text[], const int line)
{
    chip.setCursor(0, line);
    chip.print(text);
    chip.print(m_Buffer.data());
}

void Hardware::LCD::Clear()
{
    chip.clear();
}