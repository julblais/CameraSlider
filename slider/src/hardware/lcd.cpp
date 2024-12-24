
#include "lcd.h"
#include "src/output/displayBuffer.h"

#include <LiquidCrystal_I2C.h>
#include <array>

#include "src/debug.h"

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
{} 

void Hardware::LCD::Init()
{
    Debug.Log("Init LCD.");
    chip.init();
    
    auto id = 0;
    CreateSymbol(id, DoubleLeftRightArrows);
    m_DoubleLeftRightArrows = id++;
    CreateSymbol(id, DoubleUpDownArrows);
    m_DoubleUpDownArrows = id++;
    
    chip.backlight();
}

void Hardware::LCD::CreateSymbol(const int id, const uint8_t *charmap)
{
    Debug.Log("Creating custom character with id: ", id);
    chip.createChar(id, const_cast<uint8_t*>(charmap));
}

void Hardware::LCD::Clear()
{
    chip.clear();
}

void Hardware::LCD::Write(uint8_t value)
{
    chip.write(value);
}

void Hardware::LCD::SetCursor(const int column, const int row)
{
    chip.setCursor(column, row);
}

Output::SymbolHandle Hardware::LCD::GetSymbol(Symbol symbol) const
{
    using namespace Output;
    switch (symbol)
    {
        case Symbol::DoubleLeftRightArrows:
            return SymbolHandle(m_DoubleLeftRightArrows);
        case Symbol::DoubleUpDownArrows:
            return SymbolHandle(m_DoubleUpDownArrows);
    };
    return SymbolHandle(-1);
}
