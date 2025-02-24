#ifndef MESSAGES_H
#define MESSAGES_H

#include <Print.h>
#include "src/core/network/address.h"

namespace Net
{
    struct ConnectionRequest : public Printable
    {
        uint8_t from[6];

        ConnectionRequest(const Core::MacAddress& address);
        virtual size_t printTo(Print& p) const override;
    };

    struct Handshake : public Printable
    {
        uint8_t from[6];

        Handshake(const Core::MacAddress& address);
        virtual size_t printTo(Print& p) const override;
    };
}

#endif