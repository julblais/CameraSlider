
#ifndef IO_SYMBOLS_H
#define IO_SYMBOLS_H

#include <Print.h>

namespace IO
{
    enum class Symbol
    {
        LeftRightArrows = 0,
        UpDownArrows = 1,
        LeftArrow = 2,
        RightArrow = 3,
        COUNT
    };

    class SymbolHandle : public Printable
    {
    public:
        SymbolHandle(Symbol symbol) :
            m_Id(static_cast<uint8_t>(symbol)) {}

        size_t printTo(Print& p) const override
        {
            return p.write(m_Id);
        }

    private:
        uint8_t m_Id;
    };

    SymbolHandle GetSymbol(Symbol symbol);
}

#endif //SLIDER_SYMBOLS_H
