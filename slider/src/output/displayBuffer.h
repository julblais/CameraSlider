#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include "src/core/output/display.h"

#include <array>
#include <Print.h>

namespace Output
{
    using Keycode = uint8_t;

    class DisplayBuffer : public Display
    {
        constexpr static auto LCD_LINE_LENGTH = 16;
        constexpr static auto LCD_NUM_LINES = 2;

        using ConstIterator =
            std::array<Keycode, LCD_LINE_LENGTH* LCD_NUM_LINES>::const_iterator;

    public:
        DisplayBuffer();
        virtual ~DisplayBuffer() = default;
        void Init(Display* display);

        virtual size_t write(uint8_t value) override;
        virtual void SetCursor(const int column, const int row) override;
        virtual SymbolHandle GetSymbol(Symbol symbol) const override;
        virtual void Clear() override;

        void PrintToDisplay() const;

        inline ConstIterator begin() const { return m_Buffer.begin(); }
        inline ConstIterator end() const { return m_Buffer.end(); }
    protected:
        virtual void FillCurrentLine() override;
    private:
        unsigned int m_Cursor;
        Display* m_Display;
        std::array<Keycode, LCD_LINE_LENGTH* LCD_NUM_LINES> m_Buffer;
        mutable std::array<Keycode, LCD_LINE_LENGTH* LCD_NUM_LINES> m_PreviousBuffer;
    };
}

#endif