#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

namespace Hardware{

  class LCD
  {
    public:
      static const uint8_t NUM_COLS = 16;
      static const uint8_t NUM_ROWS = 2;

      LCD(const uint8_t address);
      void Init();
      void Clear();
      void Print(const char text[], const int line);

    private:
      LiquidCrystal_I2C chip;
  };
}

#endif