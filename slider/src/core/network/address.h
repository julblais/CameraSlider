#ifndef ADDRESS_H
#define ADDRESS_H

#include <array>
#include <Print.h>
#include <Printable.h>

namespace Core
{
    struct MacAddress : public Printable
    {
    public:
        MacAddress();
        MacAddress(const std::array<uint8_t, 6> address);
        MacAddress(const uint8_t* address);

        void CopyTo(uint8_t* dest) const;
        const size_t Size() const { return m_Address.size(); }
        const uint8_t* Data() const { return m_Address.data(); }
        virtual size_t printTo(Print& p) const override;

    protected:
        std::array<uint8_t, 6> m_Address;
    };

    const MacAddress BROADCAST_ADDRESS { {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF} };
    const MacAddress INVALID_ADDRESS { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
}

#endif