#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "core/display.h"

namespace Hardware
{
    class LCD : public Core::Display
    {
    public:
        constexpr static int NUM_COLS = 16;
        constexpr static int NUM_ROWS = 2;

        LCD(const uint8_t address);

        void Clear() override;
        size_t write(uint8_t value) override;
        void SetCursor(const int column, const int row) override;
        Core::SymbolHandle GetSymbol(Core::Symbol symbol) const override;

    private:
        void CreateSymbol(const int id, const uint8_t* charmap);

        LiquidCrystal_I2C chip;
        Core::Keycode m_DoubleLeftRightArrows;
        Core::Keycode m_DoubleUpDownArrows;
        Core::Keycode m_LeftArrow;
        Core::Keycode m_RightArrow;
    };
}

#endif
