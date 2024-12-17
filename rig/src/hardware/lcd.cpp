
#include "hardware/lcd.h"

#include <LiquidCrystal_I2C.h>
#include <array>

#include "debug.h"

std::array<char, Hardware::LCD::NUM_COLS> m_Buffer;
static uint8_t s_IdCount = 0;

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

Hardware::LCD::CustomChar::CustomChar(LCD *lcd, const uint8_t charmap[])
    : m_Id(s_IdCount++)
{
    lcd->chip.createChar(m_Id, const_cast<uint8_t*>(charmap));
    Debug.Log("Creating custom character with id: ", m_Id);
}

void Hardware::LCD::Write(const CustomChar &customChar, const int column, const int row)
{
    chip.setCursor(column, row);
    chip.write(customChar.m_Id);
}