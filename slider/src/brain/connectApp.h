#ifndef BRAINCONNECTAPP_H
#define BRAINCONNECTAPP_H

#include "src/core/app/appBase.h"
#include "src/core/time/timer.h"
#include "src/core/app/appBase.h"
#include "src/core/network/address.h"
#include "src/core/network/messageCallbackHandle.h"
#include <vector>

namespace Net { class WifiModule; }

namespace Slider
{
    class BrainConnector : public Core::Component
    {
        enum class ConnectionState
        {
            BROADCASTING,
            SENDING_HANDSHAKE,
            WAITING_FOR_HANDSHAKE,
            CONNECTED
        };

    public:
        BrainConnector(Net::WifiModule* const wifi);
        ~BrainConnector();
        virtual void Setup() override;
        virtual void Update() override;

    private:
        Net::WifiModule* const m_Wifi;
        ConnectionState state;
        bool isComplete;
        Core::MacAddress controllerMac;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };

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
        ControllerConnector(Net::WifiModule* const wifi);
        ~ControllerConnector();
        virtual void Setup() override;
        virtual void Update() override;

    private:
        Net::WifiModule* const m_Wifi;
        ConnectionState state;
        bool isComplete;
        Core::MacAddress brainMac;
        Core::Timer m_Timeout;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };
}

#endif