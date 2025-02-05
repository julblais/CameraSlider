#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <functional>

#include "src/utils/templateUtils.h"
#include "address.h"
#include "src/debug.h"
#include "message.h"

namespace Network
{
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

            static void Send(const uint8_t *data, size_t len);
            static void Send(const MacAddress& address, const uint8_t* data, size_t len);
            
            template <typename Message>
            static void Send(const MacAddress& address, const Message& message);

            template <typename Message>
            static void Send(const Message& message);
    };

    template <typename Message>
    inline void Esp::Send(const MacAddress &address, const Message &message)
    {
        Send(address, reinterpret_cast<const uint8_t*>(&message), sizeof(Message));
    }

    template <typename Message>
    inline void Esp::Send(const Message &message)
    {
        Send(reinterpret_cast<const uint8_t*>(&message), sizeof(Message));
    }
}

#endif