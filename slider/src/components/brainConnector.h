#ifndef CONNECTOR_BRAIN_H
#define CONNECTOR_BRAIN_H

#include "src/core/app/appBase.h"
#include "src/core/time/timer.h"
#include "src/core/app/appBase.h"
#include "src/core/network/address.h"
#include "src/core/network/messageCallbackHandle.h"
#include <vector>

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
        BrainConnector();
        ~BrainConnector();
        virtual void Setup() override;
        virtual void Update() override;

    private:
        ConnectionState state;
        bool isComplete;
        Core::MacAddress controllerMac;
        std::vector<Core::MessageCallbackHandle> m_Callbacks;
    };
}

#endif