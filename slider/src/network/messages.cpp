#ifdef USE_NETWORK

#include "messages.h"

using namespace Core;
using namespace Net;

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

#endif
