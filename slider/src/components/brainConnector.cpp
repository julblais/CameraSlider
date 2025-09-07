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

BrainConnector::BrainConnector() :
    state(State::IDLE),
    m_BroadcastActive(false)
{
    m_BroadcastTimer = Timer::Create("Broadcast", [this]() {
        LogInfo("Sending connection request...");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(BROADCAST_ADDRESS, request);
    });
}

void BrainConnector::Setup()
{
    if (Settings::GetInstance().HasPeerAddress())
    {
        auto peer = Settings::GetInstance().GetPeerAddress();
        LogInfo("Connecting to: ", peer);
        WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);
        state = State::CONNECTED;
    }
}

void BrainConnector::BeginConnectionAttempt()
{
    if (state != State::IDLE)
        return;

    LogInfo("Starting broadcast...");
    SetupBroadcast();
    m_BroadcastActive = true;
    state = State::BROADCASTING;
}

void BrainConnector::EndConnectionAttempt()
{
    if (state != State::CONNECTED)
    {
        EndBroadcast();
        state = State::IDLE;
    }
}

void BrainConnector::SetupBroadcast()
{
    const auto connect = WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("Connection received",
        [this](ConnectionRequest msg) {
            OnConnectionReceived(msg);
        });
    const auto handshake = WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("Handshake received",
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
    for (const auto& handle: m_Callbacks)
        WifiModule::GetInstance().RemoveReceiveCallback(handle);
    m_Callbacks.clear();
    m_BroadcastActive = false;
}

void BrainConnector::OnConnectionReceived(const ConnectionRequest& message)
{
    if (state != State::BROADCASTING)
        return;
    LogInfo("Received: ", message);
    m_BroadcastTimer.Stop();
    state = State::SENDING_HANDSHAKE;
    controllerMac = message.from;
}

void BrainConnector::OnHandshakeReceived(const Handshake& message)
{
    if (state != State::WAITING_FOR_HANDSHAKE)
        return;
    LogInfo("Received: ", message);
    Settings::GetInstance().SetPeerAddress(message.from);
    state = State::CONNECTED;
}

void BrainConnector::Update()
{
    if (state == State::SENDING_HANDSHAKE)
    {
        WifiModule::GetInstance().RemovePeer(BROADCAST_ADDRESS);
        WifiModule::GetInstance().AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        const Handshake handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = State::WAITING_FOR_HANDSHAKE;
    }
    else if (state == State::CONNECTED && m_BroadcastActive)
    {
        EndBroadcast();
        //remove timer
    }
}
