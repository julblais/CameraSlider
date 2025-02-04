#ifndef ESP_H
#define ESP_H

#include <array>
#include <functional>

#include "address.h"

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
            static void Send();
    };
}

#endif