#ifndef CONNECTOR_BRAIN_H
#define CONNECTOR_BRAIN_H

#include "src/core/app/appBase.h"
#include "src/core/network/address.h"
#include "src/core/network/messageCallbackHandle.h"
#include "src/core/time/timer.h"
#include "src/network/messages.h"
#include <vector>

namespace Slider
{
    class BrainConnector : public Core::Component
    {
    public:
        enum class ConnectionState
        {
            IDLE,
            BROADCASTING,
            SENDING_HANDSHAKE,
            WAITING_FOR_HANDSHAKE,
            CONNECTED
        };

        BrainConnector();

        virtual void Setup() override;
        virtual void Update() override;

        void BeginConnectionAttempt();
        void EndConnectionAttempt();
        inline ConnectionState GetState() { return state; }
    private:
        void SetupBroadcast();
        void EndBroadcast();
        void OnConnectionReceived(const Net::ConnectionRequest& message);
        void OnHandshakeReceived(const Net::Handshake& message);

        Timer m_BroadcastTimer;
        ConnectionState state;
        bool isComplete;
        Core::MacAddress controllerMac;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };
}

#endif