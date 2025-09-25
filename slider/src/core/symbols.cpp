#include "symbols.h"

size_t Core::Symbol::printTo(Print& p) const
{
    return p.write(id);
}
