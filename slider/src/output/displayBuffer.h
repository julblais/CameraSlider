#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include <array>
#include <Print.h>
#include "display.h"

namespace Output 
{
    using Keycode = uint8_t;

    class DisplayBuffer : private Print, public Display
    {
        constexpr static auto LCD_LINE_LENGTH = 16;
        constexpr static auto LCD_NUM_LINES = 2;

        using ConstIterator =
            std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES>::const_iterator;
        using Iterator =
            std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES>::iterator;

        public:
            DisplayBuffer();
            virtual ~DisplayBuffer() = default;
            void Init(Display* display);
            
            template <typename... TArgs> void Print(TArgs&&... args);
            template <typename... TArgs> void PrintLine(const int line, TArgs&&... args);

            virtual void Write(Keycode value) override;
            virtual void SetCursor(const int column, const int row) override;
            virtual SymbolHandle GetSymbol(Symbol symbol) const override;

            void Clear();
            void PrintToDisplay() const;

            inline ConstIterator begin() const { return m_Buffer.begin(); }
            inline ConstIterator end() const { return m_Buffer.end(); }

        private:
            virtual size_t write(uint8_t value) override;
            void FillCurrentLine();

            unsigned int m_Cursor;
            Display* m_Display;
            std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_Buffer;
            mutable std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_PreviousBuffer;
    };

    //use this weird thing to avoid undefined order of variadic template arguments
    struct passArgs { template<typename ...T> passArgs(T...) {} };

    template <typename... TArgs>
    inline void DisplayBuffer::Print(TArgs &&...args)
    {
        passArgs{(print(args), 1)...};
    }

    template <typename... TArgs>
    void DisplayBuffer::PrintLine(const int line, TArgs &&...args)
    {
        SetCursor(0, line);
        passArgs{(print(args), 1)...};
        FillCurrentLine();
    }
}

#endif