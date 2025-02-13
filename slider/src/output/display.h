#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include <Printable.h>

namespace Output
{    
    using Keycode = uint8_t;

    enum class Symbol : char
    {
        LeftRightArrows,
        UpDownArrows
    };

    class SymbolHandle : public Printable
    {
        public:
            SymbolHandle(Keycode id) : m_Id(id) {};

            virtual size_t printTo(Print& p) const override
            {
                return p.write(m_Id);
            }

        private:
            uint8_t m_Id;
    };

    class Display
    {
        public:
            Display() = default;
            virtual ~Display() = default;

            virtual void Init() {}
            virtual size_t write(Keycode value) = 0;
            virtual void SetCursor(const int column, const int row) = 0;
            virtual SymbolHandle GetSymbol(Symbol symbol) const = 0;
    };
}

#endif