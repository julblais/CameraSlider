#include "address.h"

using namespace Network;

MacAddress::MacAddress(std::array<uint8_t, 6> address)
    : Address(address) {}

MacAddress::MacAddress(const uint8_t *address) 
    : Address(address){}
