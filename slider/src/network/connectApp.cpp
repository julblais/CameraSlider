#include "connectApp.h"

#include "src/debug.h"
#include "src/core/network/address.h"
#include "wifi.h"
#include "messages.h"

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
    //remove callbacks
}

void BrainConnector::Setup()
{
    LogInfo("Setup brain connector...");

    m_Wifi->RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        if (state != ConnectionState::BROADCASTING) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.from;
    });
    m_Wifi->RegisterReceiveCallback<Handshake>([this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::CONNECTED;
    });
    m_Wifi->AddPeer(BROADCAST_ADDRESS);
}

void BrainConnector::Update()
{}

//////////////////////////

ControllerConnector::ControllerConnector(Net::WifiModule* const wifi) :
    state(ConnectionState::WAITING_FOR_CONNECTION),
    isComplete(false),
    m_Wifi(wifi)
{}

ControllerConnector::~ControllerConnector()
{
    //remove callbacks
}

void ControllerConnector::Setup()
{
    LogInfo("Setup controller connector");

    m_Wifi->RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        if (state != ConnectionState::WAITING_FOR_CONNECTION) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
    });
    m_Wifi->RegisterReceiveCallback<Handshake>([this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
    });
}

void ControllerConnector::Update()
{}
