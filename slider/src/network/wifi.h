#ifndef WIFI_H
#define WIFI_H

#include "src/core/component/component.h"
#include "src/core/network/messageHandler.h"
#include "src/core/network/address.h"

namespace Net
{
    class WifiModule : public Slider::Component
    {
    public:
        virtual void Setup() override;
        virtual void Update() override;

        MacAddress GetMacAddress();
        bool AddPeer(const MacAddress& address);
        bool RemovePeer(const MacAddress& address);

        template <typename TMessage>
        void RegisterReceiveCallback(std::function<void(TMessage)> callback);
        void RegisterSendCallback(std::function<void(const MacAddress&, bool)> callback);

        template <typename TMessage>
        bool Send(const MacAddress& address, const TMessage& message);

        template <typename TMessage>
        bool Send(const TMessage& message);

    private:
        bool SendImpl(const uint8_t* address, const uint8_t* data, size_t len);
        MessageHandler m_MessageHandler;

        #ifdef IS_SIMULATOR
    private:
        MessageHandler m_OtherMessageHandler;
    public:
        template <typename TMessage>
        bool SimulateSend(const TMessage& message);
        template <typename TMessage>
        void RegisterSimulateSendCallback(std::function<void(TMessage)> callback);
        #endif
    };

    template <typename TMessage>
    void WifiModule::RegisterReceiveCallback(std::function<void(TMessage)> callback)
    {
        m_MessageHandler.AddCallback<TMessage>(callback);
    }

    template <typename TMessage>
    bool WifiModule::Send(const MacAddress& address, const TMessage& message)
    {
        auto wrapper = MessageHandler::CreateMessage(message);
        return SendImpl(address.Data(), reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
    }

    template <typename TMessage>
    bool WifiModule::Send(const TMessage& message)
    {
        auto wrapper = MessageHandler::CreateMessage(message);
        return SendImpl(nullptr, reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
    }

    #ifdef IS_SIMULATOR
    template <typename TMessage>
    void WifiModule::RegisterSimulateSendCallback(std::function<void(TMessage)> callback)
    {
        m_OtherMessageHandler.AddCallback<TMessage>(callback);
    }

    template <typename Message>
    bool WifiModule::SimulateSend(const Message& message)
    {
        auto wrapper = MessageHandler::CreateMessage(message);
        m_MessageHandler.Invoke(reinterpret_cast<const uint8_t*>(&wrapper), sizeof(wrapper));
        return true;
    }
    #endif
}

#endif