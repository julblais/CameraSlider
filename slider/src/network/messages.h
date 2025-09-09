#ifndef MESSAGES_H
#define MESSAGES_H

#ifdef USE_NETWORK

#include <Print.h>
#include "src/core/network/address.h"
#include "src/core/network/message.h"

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

REGISTER_MESSAGE_TYPE(Net::ConnectionRequest, 1);
REGISTER_MESSAGE_TYPE(Net::Handshake, 2);

#endif
#endif
