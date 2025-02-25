#include "netApp.h"
#include "appConfig.h"
#include "src/debug.h"
#include "src/core/network/address.h"
#include "src/network/wifiModule.h"
#include "src/network/wifiComponent.h"

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

struct ConnectionRequest2 : public Printable
{
    uint8_t from[6];

    ConnectionRequest2(const MacAddress& address)
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

struct Handshake2 : public Printable
{
    uint8_t from[6];

    Handshake2(const MacAddress& address)
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

REGISTER_MESSAGE_TYPE(ConnectionRequest2, 1);
REGISTER_MESSAGE_TYPE(InputMessage, 2);
REGISTER_MESSAGE_TYPE(Handshake2, 3);

BrainApp::BrainApp(const AppConfig& config)
    : state(ConnectionState::BROADCASTING),
    isComplete(false),
    m_Timeout()
{}

void BrainApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    AddComponent<WifiComponent>();
    SetupComponents();

    WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest2>("connection", [this](ConnectionRequest2 msg) {
        if (state != ConnectionState::BROADCASTING) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.from;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<Handshake2>("handshake", [this](Handshake2 msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::CONNECTED;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<InputMessage>("input", [this](InputMessage msg) {
        LogInfo("Received: ", msg);
        isComplete = state == ConnectionState::CONNECTED;
    });
    WifiModule::GetInstance().AddPeer(BROADCAST_ADDRESS);

    #ifdef IS_SIMULATOR
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest2>("connection", [this](ConnectionRequest2 msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        ConnectionRequest2 connectMsg(otherMac);
        WifiModule::GetInstance().SimulateSend(msg);
    });
    WifiModule::GetInstance().RegisterSimulateSendCallback<Handshake2>("handshake", [this](Handshake2 msg) {
        Handshake2 handshake(MacAddress({ 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }));
        WifiModule::GetInstance().SimulateSend(handshake);
        InputMessage inoutMsg;
        inoutMsg.x = 10;
        inoutMsg.y = 20;
        WifiModule::GetInstance().SimulateSend(inoutMsg);
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
        ConnectionRequest2 request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(BROADCAST_ADDRESS, request);
        delay(BRAIN_BROADCAST_DELAY);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        //add a timer here to return to BROADCASTING after a while + add broadcast peer again
        WifiModule::GetInstance().RemovePeer(BROADCAST_ADDRESS);
        WifiModule::GetInstance().AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        Handshake2 handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        //remove timer
    }
}

ControllerApp::ControllerApp(const AppConfig& config)
    : state(ConnectionState::WAITING_FOR_CONNECTION), isComplete(false)
{}

void ControllerApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    LogInfo("Init wifi...");
    AddComponent<WifiComponent>();
    SetupComponents();

    WifiModule::GetInstance().RegisterReceiveCallback<ConnectionRequest2>("connection", [this](ConnectionRequest2 msg) {
        if (state != ConnectionState::WAITING_FOR_CONNECTION) return;
        LogInfo("Received: ", msg);
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
    });
    WifiModule::GetInstance().RegisterReceiveCallback<Handshake2>("input", [this](Handshake2 msg) {
        if (state != ConnectionState::WAITING_FOR_HANDSHAKE) return;
        LogInfo("Received: ", msg);
        state = SENDING_HANDSHAKE;
    });

    #ifdef IS_SIMULATOR
    WifiModule::GetInstance().RegisterSimulateSendCallback<ConnectionRequest2>("connection", [this](ConnectionRequest2 msg) {
        MacAddress otherMac { {0x11, 0x22, 0x33, 0x44, 0x55, 0x66} };
        Handshake2 hand(otherMac);
        WifiModule::GetInstance().SimulateSend(hand);
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
        ConnectionRequest2 msg(receiverMac);
        WifiModule::GetInstance().SimulateSend(msg);
        #endif
    }

    if (state == ConnectionState::SENDING_REQUEST)
    {
        WifiModule::GetInstance().AddPeer(brainMac);
        LogInfo("Sending connection request.");
        ConnectionRequest2 request(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(request);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
        return;
    }
    if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        Handshake2 handshake(WifiModule::GetInstance().GetMacAddress());
        WifiModule::GetInstance().Send(handshake);
        state = ConnectionState::CONNECTED;
        return;
    }
    if (state == ConnectionState::CONNECTED && !isComplete)
    {
        LogInfo("Sending input message");
        InputMessage msg;
        msg.x = 10;
        msg.y = 20;
        WifiModule::GetInstance().Send(msg);
        isComplete = true;
    }
}