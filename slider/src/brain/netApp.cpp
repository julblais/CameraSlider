#include "netApp.h"
#include "appConfig.h"
#include "src/debug.h"
#include "src/network/address.h"
#include "src/network/wifi.h"

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

struct ConnectionRequest : public Printable
{
    uint8_t from[6];

    ConnectionRequest(const MacAddress& address)
    {
        address.CopyTo(&from[0]);
    }

    virtual size_t printTo(Print& p) const override
    {
        auto s = p.print("-ConnectionRequest- from: ");
        s += p.print(MacAddress(from));
        return s;
    }
};

struct Handshake : public Printable
{
    uint8_t from[6];

    Handshake(const MacAddress& address)
    {
        address.CopyTo(&from[0]);
    }

    virtual size_t printTo(Print& p) const override
    {
        auto s = p.print("-Handshake- from: ");
        s += p.print(MacAddress(from));
        return s;
    }
};

struct InputMessage : public Printable
{
    int x;
    int y;

    virtual size_t printTo(Print& p) const override
    {
        return p.printf("-InputMessage- {x: %i, y: %i}", x, y);
    }
};

REGISTER_MESSAGE_TYPE(ConnectionRequest, 1);
REGISTER_MESSAGE_TYPE(InputMessage, 2);
REGISTER_MESSAGE_TYPE(Handshake, 3);

BrainApp::BrainApp(const AppConfig& config)
    : state(ConnectionState::BROADCASTING), isComplete(false), m_Wifi(nullptr)
{}

void BrainApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    m_Wifi = new WifiModule();
    AddComponent(m_Wifi);
    SetupComponents();

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
    m_Wifi->RegisterReceiveCallback<InputMessage>([this](InputMessage msg) {
        LogInfo("Received: ", msg);
        isComplete = state == ConnectionState::CONNECTED;
    });
    m_Wifi->AddPeer(BROADCAST_ADDRESS);

    #ifdef IS_SIMULATOR
    m_Wifi->RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        ConnectionRequest connectMsg(otherMac);
        m_Wifi->SimulateSend(msg);
    });
    m_Wifi->RegisterSimulateSendCallback<Handshake>([this](Handshake msg) {
        Handshake handshake(MacAddress({ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }));
        m_Wifi->SimulateSend(handshake);
        InputMessage inoutMsg;
        inoutMsg.x = 10;
        inoutMsg.y = 20;
        m_Wifi->SimulateSend(inoutMsg);
        delay(1000);
    });
    #endif
}

void BrainApp::Update()
{
    UpdateComponents();

    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::BROADCASTING)
    {
        LogInfo("Sending connection request...");
        ConnectionRequest request(m_Wifi->GetMacAddress());
        m_Wifi->Send(BROADCAST_ADDRESS, request);
        delay(BRAIN_BROADCAST_DELAY);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        //add a timer here to return to BROADCASTING after a while + add broadcast peer again
        m_Wifi->RemovePeer(BROADCAST_ADDRESS);
        m_Wifi->AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        Handshake handshake(m_Wifi->GetMacAddress());
        m_Wifi->Send(handshake);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        //remove timer
    }
}

ControllerApp::ControllerApp(const AppConfig& config)
    : state(ConnectionState::WAITING_FOR_CONNECTION), isComplete(false), m_Wifi(nullptr)
{}

void ControllerApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    m_Wifi = new WifiModule();
    AddComponent(m_Wifi);
    SetupComponents();

    m_Wifi->RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        if (state != ConnectionState::WAITING_FOR_CONNECTION) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
    });
    m_Wifi->RegisterReceiveCallback<Handshake>([this](Handshake msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = SENDING_HANDSHAKE;
    });

    #ifdef IS_SIMULATOR
    m_Wifi->RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        Handshake hand(otherMac);
        m_Wifi->SimulateSend(hand);
    });
    #endif
}

void ControllerApp::Update()
{
    UpdateComponents();

    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::WAITING_FOR_CONNECTION)
    {
        LogInfo("Waiting for connection request");
        delay(CONTROLLER_CONNECTION_DELAY);
        #ifdef IS_SIMULATOR
                //Simulate a connection from brain
        MacAddress receiverMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        ConnectionRequest msg(receiverMac);
        m_Wifi->SimulateSend(msg);
        #endif
    }

    if (state == ConnectionState::SENDING_REQUEST)
    {
        m_Wifi->AddPeer(brainMac);
        LogInfo("Sending connection request.");
        ConnectionRequest request(m_Wifi->GetMacAddress());
        m_Wifi->Send(request);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
        return;
    }
    if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        Handshake handshake(m_Wifi->GetMacAddress());
        m_Wifi->Send(handshake);
        state = ConnectionState::CONNECTED;
        return;
    }
    if (state == ConnectionState::CONNECTED && !isComplete)
    {
        LogInfo("Sending input message");
        InputMessage msg;
        msg.x = 10;
        msg.y = 20;
        m_Wifi->Send(msg);
        isComplete = true;
    }
}