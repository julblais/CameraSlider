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

ControllerConnector::ControllerConnector() :
    state(State::WAITING_FOR_CONNECTION)
{
}

void ControllerConnector::Setup()
{
    if (Settings::GetInstance().HasPeerAddress())
    {
        auto peer = Settings::GetInstance().GetPeerAddress();
        LogInfo("Connecting to: ", peer);
        WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);
        state = State::CONNECTED;
    }
    else
    {
        BeginConnectionAttempt();
        LogInfo("Waiting for connection request from slider...");
    }
}

void ControllerConnector::BeginConnectionAttempt()
{
    auto connect = WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("Connection received",
        [this](ConnectionRequest msg) {
            OnConnectionReceived(msg);
        });
    auto handshake = WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("Handshake received",
        [this](Handshake msg) {
            OnHandshakeReceived(msg);
        });
    m_Callbacks.push_back(connect);
    m_Callbacks.push_back(handshake);
}

void ControllerConnector::EndConnectionAttempt() const
{
    for (const auto& handle: m_Callbacks)
        WifiModule::GetInstance().RemoveReceiveCallback(handle);
}

void ControllerConnector::OnConnectionReceived(const Net::ConnectionRequest& message)
{
    if (state != State::WAITING_FOR_CONNECTION)
        return;
    LogInfo("Received: ", message);
    state = State::SENDING_REQUEST;
    brainMac = message.from;
}

void ControllerConnector::OnHandshakeReceived(const Net::Handshake& message)
{
    if (state != State::WAITING_FOR_HANDSHAKE)
        return;
    LogInfo("Received: ", message);
    state = State::SENDING_HANDSHAKE;
}

void ControllerConnector::Update()
{
    if (state == State::SENDING_REQUEST)
    {
        WifiModule::GetInstance().AddPeer(brainMac);
        LogInfo("Sending connection request.");
        const ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(request);
        state = State::WAITING_FOR_HANDSHAKE;
    }
    else if (state == State::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        const Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        Settings::GetInstance().SetPeerAddress(brainMac);
        EndConnectionAttempt();
        state = State::CONNECTED;
    }
}
