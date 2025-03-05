#include "brainConnector.h"

#include "src/debug.h"
#include "src/core/network/address.h"
#include "src/network/wifiModule.h"
#include "src/network/messages.h"
#include "src/app/settings.h"

using namespace Slider;
using namespace Net;

#define BRAIN_BROADCAST_DELAY 2000

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

BrainConnector::BrainConnector()
    : state(ConnectionState::IDLE),
    isComplete(false)
{
    m_BroadcastTimer = Timer::Create("Broadcast", [this]() {
        LogInfo("Sending connection request...");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(BROADCAST_ADDRESS, request);
    });
}

void Slider::BrainConnector::Setup()
{
    if (Settings::GetInstance().HasPeerAddress())
    {
        auto peer = Settings::GetInstance().GetPeerAddress();
        LogInfo("Connecting to: ", peer);
        WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);
        state = ConnectionState::CONNECTED;
    }
}

void BrainConnector::BeginConnectionAttempt()
{
    if (state != ConnectionState::IDLE)
        return;

    LogInfo("Starting broadcast...");
    SetupBroadcast();
    state = ConnectionState::BROADCASTING;
}

void BrainConnector::EndConnectionAttempt()
{
    if (state != ConnectionState::CONNECTED)
    {
        EndBroadcast();
        state = ConnectionState::IDLE;
    }
}

void BrainConnector::SetupBroadcast()
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
    WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);
    m_BroadcastTimer.Start(BRAIN_BROADCAST_DELAY);
}

void BrainConnector::EndBroadcast()
{
    for (const auto& handle : m_Callbacks)
        WifiModule::GetInstance().RemoveReceiveCallback(handle);
    WifiModule::GetInstance().RemovePeer(BROADCAST_ADDRESS);
    m_Callbacks.clear();
}

void BrainConnector::OnConnectionReceived(const ConnectionRequest& message)
{
    if (state != ConnectionState::BROADCASTING) return;
    LogInfo("Received: ", message);
    m_BroadcastTimer.Stop();
    state = ConnectionState::SENDING_HANDSHAKE;
    controllerMac = message.from;
}

void BrainConnector::OnHandshakeReceived(const Handshake& message)
{
    if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
    LogInfo("Received: ", message);
    Slider::Settings::GetInstance().SetPeerAddress(message.from);
    state = ConnectionState::CONNECTED;
}

void BrainConnector::Update()
{
    if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        WifiModule::GetInstance().RemovePeer(BROADCAST_ADDRESS);
        WifiModule::GetInstance().AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        EndBroadcast();
        //remove timer
    }
}