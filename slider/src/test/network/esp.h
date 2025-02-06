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
            
            template <typename TMessage>
            static bool Send(const MacAddress& address, const TMessage& message);
            template <typename TMessage>
            static bool Send(const TMessage& message);
            
            template <typename TMessage>
            static bool SimulateSend(const TMessage& message);
            template <typename TMessage>
            static void RegisterSimulateSendCallback(std::function<void(TMessage)> callback);

        private:
            static bool Send(const uint8_t* address, const uint8_t *data, size_t len);
            
        static MessageHandler s_Handler;
        static Esp::SendCallback s_SendCallback;
    #ifdef IS_SIMULATOR
        static MessageHandler s_OtherDeviceHandler;
#endif
    };

    template <typename TMessage>
    bool Esp::Send(const MacAddress &address, const TMessage &message)
    {
        auto wrapper = s_Handler.CreateMessage(message);
        return Send(address.Data(), reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
    }

    template <typename TMessage>
    bool Esp::Send(const TMessage &message)
    {
        auto wrapper = s_Handler.CreateMessage(message);
        return Send(nullptr, reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
    }
    
    template <typename TMessage>
    void Esp::RegisterReceiveCallback(std::function<void(TMessage)> callback)
    {
        s_Handler.AddCallback<TMessage>(callback);
    }

    template <typename TMessage>
    void Network::Esp::RegisterSimulateSendCallback(std::function<void(TMessage)> callback)
    {
#ifdef IS_SIMULATOR
        s_OtherDeviceHandler.AddCallback<TMessage>(callback);
#else
        assert(false && "Should not use this on actual hardware");
#endif
    }

    template <typename Message>
    bool Esp::SimulateSend(const Message &message)
    {
#ifdef IS_SIMULATOR
        auto wrapper = s_Handler.CreateMessage(message);
        s_Handler.Invoke(reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
        return true;
#else
        assert(false && "Should not use this on actual hardware");
#endif
    }
}

#endif