#include "connectApp.h"

#include "src/debug.h"
#include "src/core/network/address.h"
#include "src/network/wifiModule.h"
#include "src/network/messages.h"
#include "settings.h"

using namespace Slider;
using namespace Net;

#define Led_Pin 17
#define CONTROLLER_CONNECTION_DELAY 1000
#define BRAIN_BROADCAST_DELAY 1000

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

BrainConnector::BrainConnector(Net::WifiModule* const wifi)
    : state(ConnectionState::BROADCASTING),
    isComplete(false),
    m_Wifi(wifi)
{}

BrainConnector::~BrainConnector()
{
    for (const auto& handle : m_Callbacks)
        WifiModule::GetInstance().RemoveReceiveCallback(handle);
}

void BrainConnector::Setup()
{
    LogInfo("Setup brain connector...");

    auto connect = WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest>("Brain-Connection",
        [this](ConnectionRequest msg) {
        if (state != ConnectionState::BROADCASTING) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.from;
    });
    auto handshake = WifiModule::GetInstance().RegisterReceiveCallback<Handshake>("Brain-handshake",
        [this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::CONNECTED;
    });
    m_Callbacks.push_back(connect);
    m_Callbacks.push_back(handshake);
    WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);
}

void BrainConnector::Update()
{
    if (state == ConnectionState::BROADCASTING)
    {
        LogInfo("Sending connection request...");
        ConnectionRequest request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(BROADCAST_ADDRESS, request);
        delay(BRAIN_BROADCAST_DELAY);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
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
        //remove timer
    }
}

//////////////////////////

ControllerConnector::ControllerConnector(Net::WifiModule* const wifi) :
    state(ConnectionState::WAITING_FOR_CONNECTION),
    isComplete(false),
    m_Wifi(wifi)
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
