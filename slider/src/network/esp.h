#ifndef IS_SIMULATOR

#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <functional>

#include "src/core/network/address.h"
#include "src/debug.h"

namespace Net
{
    class Esp
    {
    public:
        using SendCallback = std::function<void(const Core::MacAddress&, bool)>;
        using ReceiveCallback = std::function<void(const uint8_t*, size_t)>;

        static bool Init();

        static Core::MacAddress GetMacAddress();
        static bool AddPeer(const Core::MacAddress& address);
        static bool RemovePeer(const Core::MacAddress& address);

        static void RegisterReceiveCb(ReceiveCallback callback);
        static void RegisterSendCallback(SendCallback callback);
        static bool Send(const uint8_t* address, const uint8_t* data, size_t len);
    };
}

#endif
#endif