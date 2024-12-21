#ifndef LCD_H
#define LCD_H

#include <LiquidCrystal_I2C.h>
#include <array>
#include <type_traits>

namespace Hardware{

    class LCD
    {
        public: 
        class CustomChar 
        {
            friend class LCD;

            public:
                CustomChar();
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
            void Write(uint8_t value) { chip.write(value); }
            void SetCursor(const int column, const int row) { chip.setCursor(column, row); }
            void Write(const CustomChar& customChar, const int column=0, const int row=0);
            inline CustomChar GetDoubleLeftRightArrows() const { return m_DoubleLeftRightArrows; }
            inline CustomChar GetDoubleUpDownArrows() const { return m_DoubleUpDownArrows; }

            template <typename T1>
            void PrintLn(T1&& text, const int line)
            {
                chip.setCursor(0, line);
                WriteAny(text);
                chip.print(m_Buffer.data());
            }
            
            template <typename T1, typename T2>
            void PrintLn(T1&& text1, T2&& text2, const int line)
            {
                chip.setCursor(0, line);
                WriteAny(text1);
                WriteAny(text2);
                chip.print(m_Buffer.data());
            }
            
            template <typename T1, typename T2, typename T3>
            void PrintLn(T1&& text1, T2&& text2, T3&& text3, const int line)
            {
                chip.setCursor(0, line);
                WriteAny(text1);
                WriteAny(text2);
                WriteAny(text3);
                chip.print(m_Buffer.data());
            }
            
            template <typename T1, typename T2, typename T3, typename T4>
            void PrintLn(T1&& text1, T2&& text2, T3&& text3, T4&& text4, const int line)
            {
                chip.setCursor(0, line);
                WriteAny(text1);
                WriteAny(text2);
                WriteAny(text3);
                WriteAny(text4);
                chip.print(m_Buffer.data());
            }

        private:
            std::array<char, Hardware::LCD::NUM_COLS> m_Buffer;
            LiquidCrystal_I2C chip;
            CustomChar m_DoubleLeftRightArrows;
            CustomChar m_DoubleUpDownArrows;

            template <class Any, 
                typename std::enable_if<!std::is_same<CustomChar, 
                    typename std::remove_reference<Any>::type>::value, int>::type = 0>
            inline void WriteAny(Any&& value)
            {
                chip.print(value);
            }

            template <class CustomCharT, 
                typename std::enable_if<std::is_same<CustomChar, 
                    typename std::remove_reference<CustomCharT>::type>::value, int>::type = 0>
            inline void WriteAny(CustomCharT&& value)
            {
                chip.write(value.m_Id);
            }
  };
}

#endif