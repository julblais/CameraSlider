#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include <array>
#include <Print.h>
#include <HardwareSerial.h>

class DisplayBuffer : private Print 
{
    constexpr static auto LCD_LINE_LENGTH = 16;
    constexpr static auto LCD_NUM_LINES = 2;

    using Keycode = uint8_t;
    using ConstIterator =
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES>::const_iterator;

    public:
        DisplayBuffer() : m_Cursor(0), m_Buffer()
        {
            m_Buffer.fill(' ');
        }

        virtual ~DisplayBuffer() = default;


        void print_internal() {}

        template <typename T, typename... TArgs>
        void print_internal(T&& arg1, TArgs&&... args)
        {
            print(arg1);
            print_internal(args...);
        }

        void fillCurrentLine()
        { 
            const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
            while (m_Cursor < maxCursor)
                m_Buffer[m_Cursor++] = ' ';
        }

        virtual size_t write(Keycode value) override
        {
            //do not go over the line!
            const auto maxCursor = ((m_Cursor / LCD_LINE_LENGTH) + 1) * LCD_LINE_LENGTH;
            if (m_Cursor < maxCursor)
                m_Buffer[m_Cursor++] = value;
            return 1;
        }

        /////public api

        const ConstIterator cbegin() const { return m_Buffer.cbegin(); }
        const ConstIterator cend() const { return m_Buffer.cend(); }

        template <typename... TArgs>
        void Print(TArgs&&... args)
        {
            print_internal(args...);
        }

        template <typename... TArgs>
        void PrintLine(const int line, TArgs&&... args)
        {
            SetCursor(line);
            print_internal(args...);
            fillCurrentLine();
        }

        void SetCursor(const int line, const int column = 0)
        {
            if (line >= LCD_NUM_LINES || line < 0)
                return;
            if (column >= LCD_LINE_LENGTH || column < 0)
                return;
            m_Cursor = line * LCD_LINE_LENGTH + column;
        }




    private:
        unsigned int m_Cursor;
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_Buffer;

        //idea is: accumulate the values in a buffer
        //then send to lcd (+ send message) at the end of the frame
};

#endif