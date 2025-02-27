#include "address.h"

using namespace Core;

MacAddress::MacAddress(std::array<uint8_t, 6> address)
    : m_Address(address)
{}

MacAddress::MacAddress(const uint8_t* address)
{
    std::copy(address, address + 6, m_Address.begin());
}

void MacAddress::CopyTo(uint8_t* dest) const
{
    std::copy(m_Address.begin(), m_Address.end(), dest);
}

size_t MacAddress::printTo(Print& p) const
{
    size_t n = 0;
    for (int i = 0; i < 6; i++)
        n += p.printf("%02x", m_Address[i]);
    return n;
}
