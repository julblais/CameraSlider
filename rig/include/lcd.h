#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

namespace Hardware{

  class LCD
  {
    static const uint8_t NUM_COLS = 16;
    static const uint8_t NUM_ROWS = 2;

    public:
      LCD(const uint8_t address);
      void Init();
      void Clear();
      template<typename T> void Print(T&& text, const int line);

    private:
      LiquidCrystal_I2C chip;
  };

  template<typename T>
  void Hardware::LCD::Print(T&& text, const int line)
  {
      chip.setCursor(0, line);
      chip.print(text);
  }
}

#endif