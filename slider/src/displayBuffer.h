#ifndef DISPLAY_BUFFER_H
#define DISPLAY_BUFFER_H

#include "core/display.h"

#include <array>
#include <Print.h>

namespace IO
{
    using Keycode = uint8_t;

    class DisplayBuffer : public Core::Display
    {
        constexpr static auto LCD_LINE_LENGTH = 16;
        constexpr static auto LCD_NUM_LINES = 2;

        using ConstIterator =
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES>::const_iterator;

    public:
        DisplayBuffer();
        ~DisplayBuffer() override = default;

        void Setup(Display* display);
        size_t write(uint8_t value) override;
        void SetCursor(const int column, const int row) override;
        Core::SymbolHandle GetSymbol(Core::Symbol symbol) const override;
        void Clear() override;
        void PrintToDisplay() const;

        ConstIterator begin() const { return m_Buffer.begin(); }
        ConstIterator end() const { return m_Buffer.end(); }

    protected:
        void FillCurrentLine() override;

    private:
        unsigned int m_Cursor;
        Display* m_Display;
        std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_Buffer;
        mutable std::array<Keycode, LCD_LINE_LENGTH * LCD_NUM_LINES> m_PreviousBuffer;
    };
}

#endif
