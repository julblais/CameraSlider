#ifndef CORE_SYMBOLS_H
#define CORE_SYMBOLS_H
#include <Print.h>

namespace Core
{
    using SymbolId = uint8_t;

    class Symbol
    {
    public:
        constexpr Symbol(const SymbolId symbol) : id(symbol) {}
        size_t printTo(Print& p) const;

        const SymbolId id;
    };

    namespace Symbols
    {
        constexpr auto LeftRightArrows{Symbol(0)};
        constexpr auto UpDownArrows{Symbol(1)};
        constexpr auto LeftArrow{Symbol(2)};
        constexpr auto RightArrow{Symbol(3)};
    }
}

#endif
