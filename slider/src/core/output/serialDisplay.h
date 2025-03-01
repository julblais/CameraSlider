#ifndef SERIALDISPLAY_H
#define SERIALDISPLAY_H

#include <array>
#include "display.h"
#include "src/core/time/timer.h"

namespace Core
{
    class SerialDisplay : public Display
    {
        constexpr static auto LCD_LINE_LENGTH = 16;
        constexpr static auto LCD_NUM_LINES = 2;
    public:
        virtual void Init() override;
        virtual size_t write(uint8_t value) override;
        virtual void SetCursor(const int column, const int row) override;
        virtual SymbolHandle GetSymbol(Symbol symbol) const override;
        virtual void Clear() override;
    protected:
        virtual void FillCurrentLine() override;
    private:
        void PrintToSerial() const;

        Timer m_Timer;
        std::array<uint8_t, LCD_LINE_LENGTH* LCD_NUM_LINES> m_Buffer;
        mutable std::array<Keycode, LCD_LINE_LENGTH* LCD_NUM_LINES> m_PreviousBuffer;
        unsigned int m_Cursor;
    };
}

#endif