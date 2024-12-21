#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include <array>
#include <Print.h>

class DisplayBuffer : private Print 
{
    constexpr static auto LCD_LINE_LENGTH = 16;
    constexpr static auto LCD_NUM_LINES = 2;

    using Keycode = uint8_t;
    using ConstIterator =
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES>::const_iterator;

    public:
        DisplayBuffer();
        virtual ~DisplayBuffer() = default;

        template <typename... TArgs> void Print(TArgs&&... args);
        template <typename... TArgs> void PrintLine(const int line, TArgs&&... args);

        void SetCursor(const int line, const int column = 0);

        inline const ConstIterator cbegin() const { return m_Buffer.cbegin(); }
        inline const ConstIterator cend() const { return m_Buffer.cend(); }

    private:
        virtual size_t write(Keycode value) override;

        template <typename T, typename... TArgs>
        void print_internal(T&& arg1, TArgs&&... args);

        inline void print_internal() {}
        void fillCurrentLine();

        unsigned int m_Cursor;
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_Buffer;
};

template <typename T, typename... TArgs>
void DisplayBuffer::print_internal(T &&arg1, TArgs &&...args)
{
    print(arg1);
    print_internal(args...);
}

template <typename... TArgs>
void DisplayBuffer::Print(TArgs &&...args)
{
    print_internal(args...);
}

template <typename... TArgs>
void DisplayBuffer::PrintLine(const int line, TArgs &&...args)
{
    SetCursor(line);
    print_internal(args...);
    fillCurrentLine();
}

#endif