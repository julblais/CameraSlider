#ifndef CORE_SYMBOLS_H
#define CORE_SYMBOLS_H
#include <Print.h>

namespace Core
{
    using SymbolId = uint8_t;

    class Symbol : public Printable
    {
    public:
        Symbol(const SymbolId symbol);
        size_t printTo(Print& p) const override;

        const SymbolId id;
    };

    namespace Symbols
    {
        const auto LeftRightArrows{Symbol(0)};
        const auto UpDownArrows{Symbol(1)};
        const auto LeftArrow{Symbol(2)};
        const auto RightArrow{Symbol(3)};
    }
}

#endif
