#ifndef DISPLAY_H
#define DISPLAY_H

#include <Print.h>
#include <Printable.h>

namespace Output
{    
    using Keycode = uint8_t;

    enum class Symbol : char
    {
        DoubleLeftRightArrows,
        DoubleUpDownArrows
    };

    class SymbolHandle /*: public Printable*/
    {
        public:
            SymbolHandle(Keycode id) : m_Id(id) {};

             operator uint8_t() const { return m_Id; }
            /*virtual size_t printTo(Print& p) const override
            {
                p.write(m_Id);
                return 1;
            }*/

        private:
            uint8_t m_Id;
    };

    class Display
    {
        public:
            Display() = default;
            virtual ~Display() = default;

            virtual void Init() {}
            virtual void Write(Keycode value) = 0;
            virtual void SetCursor(const int column, const int row) = 0;
            virtual SymbolHandle GetSymbol(Symbol symbol) const = 0;
    };
}

#endif