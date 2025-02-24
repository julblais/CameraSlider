#include "messages.h"
#include "src/core/network/message.h"

using namespace Core;
using namespace Net;

REGISTER_MESSAGE_TYPE(ConnectionRequest, 1);
REGISTER_MESSAGE_TYPE(Handshake, 2);

ConnectionRequest::ConnectionRequest(const Core::MacAddress& address)
{
    address.CopyTo(&from[0]);
}

size_t ConnectionRequest::printTo(Print& p) const
{
    auto s = p.print("-ConnectionRequest- from: ");
    s += p.print(MacAddress(from));
    return s;
}

Handshake::Handshake(const MacAddress& address)
{
    address.CopyTo(&from[0]);
}

size_t Handshake::printTo(Print& p) const
{
    auto s = p.print("-Handshake- from: ");
    s += p.print(MacAddress(from));
    return s;
}