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
            constexpr static int NUM_COLS = 16;
            constexpr static int NUM_ROWS = 2;

            LCD(const uint8_t address);
            virtual void Init() override;

            void Clear();
            virtual void Write(uint8_t value) override;
            void SetCursor(const int column, const int row) override;
            virtual SymbolHandle GetSymbol(Symbol symbol) const override;
            
        private:
            void CreateSymbol(const int id, const uint8_t* charmap);

            LiquidCrystal_I2C chip;
            Keycode m_DoubleLeftRightArrows;
            Keycode m_DoubleUpDownArrows;
  };
}

#endif