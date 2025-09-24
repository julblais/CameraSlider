#include "symbols.h"

using namespace IO;

SymbolHandle IO::GetSymbol(const Symbol symbol)
{
    switch (symbol)
    {
    case Symbol::UpDownArrows:
        return SymbolHandle(Symbol::UpDownArrows);
    case Symbol::LeftArrow:
        return SymbolHandle(Symbol::LeftArrow);
    case Symbol::RightArrow:
        return SymbolHandle(Symbol::RightArrow);
    default:
        return SymbolHandle(Symbol::LeftRightArrows);
    };
}
