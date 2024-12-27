#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include "src/output/display.h"
#include <array>
#include <type_traits>

using namespace Output;

namespace Hardware{

    class LCD : public Display
    {
        public:
            constexpr static unsigned char NUM_COLS = 16;
            constexpr static unsigned char NUM_ROWS = 2;

            LCD(const uint8_t address);
            virtual void Init() override;

            void Clear();
            virtual void Write(uint8_t value) override;
            void SetCursor(const unsigned char column, const unsigned char row) override;
            virtual SymbolHandle GetSymbol(Symbol symbol) const override;
            
        private:
            void CreateSymbol(const unsigned char id, const uint8_t* charmap);

            LiquidCrystal_I2C chip;
            Keycode m_DoubleLeftRightArrows;
            Keycode m_DoubleUpDownArrows;
  };
}

#endif