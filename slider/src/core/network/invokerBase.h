#ifndef INVOKERBASE_H
#define INVOKERBASE_H

#include <inttypes.h>

namespace Core
{
    class InvokerBase
    {
    public:
        InvokerBase(const char* name) : name(name) {}
        virtual void Invoke(const uint8_t* data, size_t length) const = 0;
        virtual ~InvokerBase() = default;
        const char* name;
    };
}

#endif