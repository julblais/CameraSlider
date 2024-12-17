#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>

namespace Hardware{

    class LCD
    {
        public:
          class CustomChar 
          {
              friend class LCD;

              public:
                  CustomChar(LCD* lcd, const uint8_t charmap[]);

              private:
                  uint8_t m_Id;
          };

        public:
            static const uint8_t NUM_COLS = 16;
            static const uint8_t NUM_ROWS = 2;

            LCD(const uint8_t address);
            void Init();
            void Clear();
            void PrintLn(const char text[], const int line);
            void Write(const CustomChar& customChar, const int column=0, const int row=0);

        private:
            LiquidCrystal_I2C chip;
  };
}

#endif