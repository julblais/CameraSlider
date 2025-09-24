
#include "lcd.h"
#include "displayBuffer.h"

#include <LiquidCrystal_I2C.h>
#include <array>

#include "core/symbols.h"
#include "core/debug.h"

using namespace Core;

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
    chip(address, NUM_COLS, NUM_ROWS)
{
    LogInfo("Init LCD.");
    chip.init();

    CreateSymbol(Symbols::LeftRightArrows.id, LeftRightArrows);
    CreateSymbol(Symbols::UpDownArrows.id, UpDownArrows);
    CreateSymbol(Symbols::LeftArrow.id, LeftArrow);
    CreateSymbol(Symbols::RightArrow.id, RightArrow);

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