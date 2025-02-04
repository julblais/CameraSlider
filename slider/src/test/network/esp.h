#ifndef ESP_H
#define ESP_H

#include <array>
#include <functional>
#include <Print.h>
#include <Printable.h>

template<int Length>
struct Address : public Printable
{
    public:
        Address();
        Address(const std::array<uint8_t, Length> address);

        void CopyTo(uint8_t* dest) const;
        virtual size_t printTo(Print& p) const override;

    protected:
        std::array<uint8_t, 6> m_Address;
};

struct MacAddress : public Address<6> {
    public:
        MacAddress(std::array<uint8_t, 6> address);
        MacAddress(const uint8_t* address);
};

const MacAddress BROADCAST {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
const MacAddress INVALID {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

class Esp
{
    public:
        using ReceiveCallback = std::function<void(const MacAddress&, const uint8_t*, int)>;
        using SendCallback = std::function<void(const MacAddress&, bool)>;

        static bool Init();
        static MacAddress GetMacAddress();
        static bool AddPeer(const MacAddress& address);
        static bool RemovePeer(const MacAddress& address);
        static void RegisterReceiveCallback(ReceiveCallback callback);
        static void RegisterSendCallback(SendCallback callback);
        static void Send();
};

template <int Length>
inline Address<Length>::Address() {}

template <int Length>
inline Address<Length>::Address(const std::array<uint8_t, Length> address) 
    : m_Address(address) {}

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
        n += p.print(m_Address[i], HEX);
        if (i < Length - 1)
            n += p.print(':');
    }
    return n;
}

#endif