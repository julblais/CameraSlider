#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>

namespace Core
{
    using Keycode = uint8_t;
    constexpr char Endl{'\n'};

    class Display : public Print
    {
    public:
        virtual void SetCursor(const int column, const int row) = 0;
        virtual void Clear() = 0;

        template <typename T>
        void Print(T&& arg);

    protected:
        virtual void FillCurrentLine() {}
    };

    template <typename T>
    void Display::Print(T&& arg)
    {
        print(arg);
    }
}

template <typename T>
Core::Display& operator<<(Core::Display& display, T&& msg)
{
    display.print(msg);
    return display;
}

#endif
