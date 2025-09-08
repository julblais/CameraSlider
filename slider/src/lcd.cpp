
#include "lcd.h"
#include "displayBuffer.h"

#include <LiquidCrystal_I2C.h>
#include <array>

#include "core/debug.h"

static const uint8_t LeftRightArrows[] = {
    0b00000,
    0b00000,
    0b01010,
    0b11011,
    0b11011,
    0b01010,
    0b00000,
    0b00000
};

static const uint8_t UpDownArrows[] = {
    0b00000,
    0b01100,
    0b11110,
    0b00000,
    0b11110,
    0b01100,
    0b00000,
    0b00000,
};

static const uint8_t LeftArrow[] = {
    0b00000,
    0b00010,
    0b00110,
    0b01110,
    0b01110,
    0b00110,
    0b00010,
    0b00000
};

static const uint8_t RightArrow[] = {
    0b00000,
    0b01000,
    0b01100,
    0b01110,
    0b01110,
    0b01100,
    0b01000,
    0b00000
};

Hardware::LCD::LCD(const uint8_t address) :
    chip(address, NUM_COLS, NUM_ROWS),
    m_DoubleLeftRightArrows(0),
    m_DoubleUpDownArrows(0),
    m_LeftArrow(0),
    m_RightArrow(0)
{
    LogInfo("Init LCD.");
    chip.init();

    auto id = 0;
    CreateSymbol(id, LeftRightArrows);
    m_DoubleLeftRightArrows = id++;
    CreateSymbol(id, UpDownArrows);
    m_DoubleUpDownArrows = id++;
    CreateSymbol(id, LeftArrow);
    m_LeftArrow = id++;
    CreateSymbol(id, RightArrow);
    m_RightArrow = id++;

    chip.backlight();
}

void Hardware::LCD::CreateSymbol(const int id, const uint8_t* charmap)
{
    LogDebug("Creating custom character with id: ", id);
    chip.createChar(id, const_cast<uint8_t*>(charmap));
}

void Hardware::LCD::Clear()
{
    chip.clear();
}

size_t Hardware::LCD::write(uint8_t value)
{
    return chip.write(value);
}

void Hardware::LCD::SetCursor(const int column, const int row)
{
    chip.setCursor(column, row);
}

SymbolHandle Hardware::LCD::GetSymbol(Symbol symbol) const
{
    using namespace Output;
    switch (symbol)
    {
        case Symbol::LeftRightArrows:
            return SymbolHandle(m_DoubleLeftRightArrows);
        case Symbol::UpDownArrows:
            return SymbolHandle(m_DoubleUpDownArrows);
        case Symbol::LeftArrow:
            return SymbolHandle(m_LeftArrow);
        case Symbol::RightArrow:
            return SymbolHandle(m_RightArrow);
    };
    return SymbolHandle(-1);
}
