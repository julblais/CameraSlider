#ifndef CONNECTOR_CONTROLLER_H
#define CONNECTOR_CONTROLLER_H

#include "src/core/app/appBase.h"
#include "src/core/time/timer.h"
#include "src/core/app/appBase.h"
#include "src/core/network/address.h"
#include "src/core/network/messageCallbackHandle.h"
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
        ConnectionState state;
        bool isComplete;
        Core::MacAddress brainMac;
        Core::Timer m_Timeout;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };
}

#endif