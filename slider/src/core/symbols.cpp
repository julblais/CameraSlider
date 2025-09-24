#include "symbols.h"

Core::Symbol::Symbol(const SymbolId symbol)
    : id(symbol) {}

size_t Core::Symbol::printTo(Print& p) const
{
    return p.write(id);
}
