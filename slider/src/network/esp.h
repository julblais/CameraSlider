#ifndef IS_SIMULATOR

#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include <functional>

#include "address.h"
#include "src/debug.h"
#include "message.h"
#include "messageHandler.h"

namespace Net
{
    class Esp
    {
        public:
            using SendCallback = std::function<void(const MacAddress&, bool)>;
            using ReceiveCallback = std::function<void(const uint8_t*, size_t)>;

            static bool Init();

            static MacAddress GetMacAddress();
            static bool AddPeer(const MacAddress& address);
            static bool RemovePeer(const MacAddress& address);

            static void RegisterReceiveCb(const ReceiveCallback& callback);
            static void RegisterSendCallback(const SendCallback& callback);
            static bool Send(const uint8_t* address, const uint8_t *data, size_t len);
    };
}

#endif
#endif