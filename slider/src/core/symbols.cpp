#include "symbols.h"

Core::Symbol::Symbol(const SymbolId symbol)
    : id(symbol) {}

size_t Core::Symbol::printTo(Print& p) const
{
    if (id >= 0)
        return p.write(id);
    return 0;
}
