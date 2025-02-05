#ifdef IS_SIMULATOR

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

            template <typename TMessage>
            static void RegisterSendCallback(std::function<void(TMessage)> callback);

        private:
            static bool Send(const uint8_t *data, size_t len);
            static bool Send(const MacAddress& address, const uint8_t* data, size_t len);
            
            static MessageHandler s_ReceiveHandler;
            static MessageHandler s_SendHandler;
    };
    
    template <typename TMessage>
    void Network::Esp::RegisterSendCallback(std::function<void(TMessage)> callback)
    {
        s_SendHandler.AddCallback<TMessage>(callback);
    }

    template <typename TMessage>
    void Esp::RegisterReceiveCallback(std::function<void(TMessage)> callback)
    {
        s_ReceiveHandler.AddCallback<TMessage>(callback);
    }

    template <typename Message>
    bool Esp::Send(const MacAddress &address, const Message &message)
    {
        auto wrapper = s_ReceiveHandler.CreateMessage(message);
        return Send(address, reinterpret_cast<const uint8_t*>(&wrapper), sizeof(Message));
    }

    template <typename Message>
    bool Esp::Send(const Message &message)
    {
        auto wrapper = s_ReceiveHandler.CreateMessage(message);
        return Send(reinterpret_cast<const uint8_t*>(&wrapper), sizeof(Message));
    }
}

#endif
#endif