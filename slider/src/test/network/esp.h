#ifndef ESP_UTILS_H
#define ESP_UTILS_H

#include "address.h"
#include "src/debug.h"
#include "message.h"
#include "messageHandler.h"

#include <functional>

namespace Network
{
    class Esp
    {
        public:
            using SendCallback = std::function<void(const MacAddress&, bool)>;

            static bool Init();
            static MacAddress GetMacAddress();
            static bool AddPeer(const MacAddress& address);
            static bool RemovePeer(const MacAddress& address);
            
            template <typename TMessage>
            static void RegisterReceiveCallback(std::function<void(TMessage)> callback);

            static void RegisterSendCallback(const SendCallback& callback);
            
            template <typename Message>
            static bool Send(const MacAddress& address, const Message& message);

            template <typename Message>
            static bool Send(const Message& message);
            
#ifdef IS_SIMULATOR
            template <typename TMessage>
            static void RegisterSendCallback(std::function<void(TMessage)> callback);
#endif

        private:
            static bool Send(const uint8_t *data, size_t len);
            static bool Send(const MacAddress& address, const uint8_t* data, size_t len);
    };
}

#endif