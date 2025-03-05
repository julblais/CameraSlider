#include "controllerConnector.h"

#include "src/debug.h"
#include "src/core/network/address.h"
#include "src/network/wifiModule.h"
#include "src/network/messages.h"
#include "src/app/settings.h"

using namespace Slider;
using namespace Net;

#define CONTROLLER_CONNECTION_DELAY 1000

/*
                 ┌─────────┐                              ┌────────────┐
                 │         │                              │            │
                 │ Slider  │                              │ Controller │
                 │         │                              │            │
                 └─────┬───┘                              └─────┬──────┘
                       │                                        │
         BROADCASTING  │   ConnectionRequest                    │  WAITING_FOR_CONNECTION
                       ├───────────────────────────────────────►│
                       │                                        │
                       │                    ConnectionRequest   │  SENDING_REQUEST
                       │◄───────────────────────────────────────┤
                       │                                        │
    SENDING_HANDSHAKE  │   Handshake                            │  WAITING_FOR_HANDSHAKE
                       ├───────────────────────────────────────►│
                       │                                        │
WAITING_FOR_HANDSHAKE  │                            Handshake   │  SENDING_HANDSHAKE
                       │◄───────────────────────────────────────┤
            CONNECTED  │                                        │  CONNECTED
 */

//const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};

ControllerConnector::ControllerConnector() :
    state(ConnectionState::WAITING_FOR_CONNECTION),
    isComplete(false)
{}

ControllerConnector::~ControllerConnector()
{
    for (const auto& handle : m_Callbacks)
        WifiModule::GetInstance().RemoveReceiveCallback(handle);
}

void ControllerConnector::Setup()
{
    LogInfo("Setup controller connector");

    auto connect = WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("Controller-connection",
        [this](ConnectionRequest msg) {
        if (state != ConnectionState::WAITING_FOR_CONNECTION) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
    });
    auto handshake = WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("Controller-handshake",
        [this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
    });
    m_Callbacks.push_back(connect);
    m_Callbacks.push_back(handshake);
}

void ControllerConnector::Update()
{
    if (state == ConnectionState::WAITING_FOR_CONNECTION)
    {
        LogInfo("Waiting for connection request");
        delay(CONTROLLER_CONNECTION_DELAY);
    }
    else if (state == ConnectionState::SENDING_REQUEST)
    {
        WifiModule::GetInstance().AddPeer(brainMac);
        LogInfo("Sending connection request.");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(request);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        Slider::Settings::GetInstance().SetPeerAddress(brainMac);
        state = ConnectionState::CONNECTED;
    }
}
