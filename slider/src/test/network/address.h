#ifndef ADDRESS_H
#define ADDRESS_H

#include <array>
#include <Print.h>
#include <Printable.h>

namespace Net
{
    template<int Length>
    struct Address : public Printable
    {
        public:
            Address();
            Address(const std::array<uint8_t, Length> address);
            Address(const uint8_t* address);

            void CopyTo(uint8_t* dest) const;
            const uint8_t* Data() const { return m_Address.data(); }
            virtual size_t printTo(Print& p) const override;

        protected:
            std::array<uint8_t, 6> m_Address;
    };

    struct MacAddress : public Address<6> {
        public:
            MacAddress();
            MacAddress(std::array<uint8_t, 6> address);
            MacAddress(const uint8_t* address);
    };

    const MacAddress BROADCAST_ADDRESS {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
    const MacAddress INVALID_ADDRESS {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    template <int Length>
    inline Address<Length>::Address() {}

    template <int Length>
    inline Address<Length>::Address(const std::array<uint8_t, Length> address) 
        : m_Address(address) {}

    template <int Length>
    inline Address<Length>::Address(const uint8_t *address)
    {
        std::copy(address, address + Length, m_Address.begin());
    }

    template <int Length>
    inline void Address<Length>::CopyTo(uint8_t *dest) const
    {
        std::copy(m_Address.begin(), m_Address.end(), dest);
    }

    template <int Length>
    inline size_t Address<Length>::printTo(Print &p) const
    {
        size_t n = 0;
        for (int i = 0; i < Length; i++)
        {
            n += p.printf("0x%02x", m_Address[i]);
            if (i < Length - 1)
                n += p.print(':');
        }
        return n;
    }
}

#endif