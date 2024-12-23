
#include "lcd.h"
#include "src/output/displayBuffer.h"

#include <LiquidCrystal_I2C.h>
#include <array>

#include "src/debug.h"

static uint8_t s_IdCount = 0;

static const uint8_t DoubleLeftRightArrows[] = {
    0b00000, 
    0b00000, 
    0b01010, 
    0b11011, 
    0b11011, 
    0b01010, 
    0b00000, 
    0b00000
};

static const uint8_t DoubleUpDownArrows[] = {
    0b00000,
    0b01100,
    0b11110,
    0b00000,
    0b11110,
    0b01100,
    0b00000,
    0b00000,
};

Hardware::LCD::LCD(const uint8_t address):
    chip(address, LCD::NUM_COLS, LCD::NUM_ROWS)
{
} 

void Hardware::LCD::Init()
{
    m_Buffer.fill(' ');

    Debug.Log("Init LCD.");
    chip.init();
    
    m_DoubleLeftRightArrows = LCD::CustomChar(this, DoubleLeftRightArrows);
    m_DoubleUpDownArrows =  LCD::CustomChar(this, DoubleUpDownArrows);
    
    chip.backlight();
}

void Hardware::LCD::Clear()
{
    chip.clear();
}

Hardware::LCD::CustomChar::CustomChar()
    : m_Id(0){}

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

void Hardware::LCD::PrintBuffer(const Output::DisplayBuffer &buffer)
{
    chip.setCursor(0, 0);
    unsigned int count = 0;
    unsigned int line = 0;
    for (const auto it : buffer)
    {
        if (count >= LCD::NUM_COLS)
        {
            chip.setCursor(0, ++line);
            count = 0;
        }
        count++;
        chip.write(it);
    }
}
