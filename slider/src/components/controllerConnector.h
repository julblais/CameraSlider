#ifndef CONNECTOR_CONTROLLER_H
#define CONNECTOR_CONTROLLER_H

#include "src/core/component/component.h"
#include "src/core/network/address.h"
#include "src/core/network/messageCallbackHandle.h"
#include "src/network/messages.h"
#include <vector>

namespace Slider
{
    class ControllerConnector : public Core::Component
    {
        enum class ConnectionState
        {
            WAITING_FOR_CONNECTION,
            SENDING_HANDSHAKE,
            SENDING_REQUEST,
            WAITING_FOR_HANDSHAKE,
            CONNECTED
        };

    public:
        ControllerConnector();
        ~ControllerConnector();
        virtual void Setup() override;
        virtual void Update() override;

    private:
        void BeginConnectionAttempt();
        void EndConnectionAttempt();
        void OnConnectionReceived(const Net::ConnectionRequest& message);
        void OnHandshakeReceived(const Net::Handshake& message);

        ConnectionState state;
        Core::MacAddress brainMac;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };
}

#endif