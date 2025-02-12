#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"
#include "messageHandler.h"
#include "address.h"

#include "esp.h"
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
        return p.print("ConnectionRequest");
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
        return p.print("Handshake");
    }
};

struct InputMessage
{
    int x;
    int y;
};

REGISTER_MESSAGE_TYPE(ConnectionRequest, 1);
REGISTER_MESSAGE_TYPE(InputMessage, 2);
REGISTER_MESSAGE_TYPE(Handshake, 3);

BrainApp::BrainApp(const AppConfig &config)
    : state(ConnectionState::BROADCASTING), isComplete(false) {}

void BrainApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

    Esp::RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        LogDebug("Received: ", msg);
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.from;
    });
    Esp::RegisterReceiveCallback<Handshake>([this](Handshake msg) {
        LogDebug("Received: ", msg);
        state = ConnectionState::CONNECTED;
    });
    Esp::RegisterReceiveCallback<InputMessage>([this](InputMessage msg) {
        isComplete = state == ConnectionState::CONNECTED;
        LogInfo("Received message x:", msg.x, " y:", msg.y);
    });
    Esp::AddPeer(BROADCAST_ADDRESS);

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}}; 
        ConnectionRequest connectMsg(otherMac);
        Esp::SimulateSend(msg);
     });
     Esp::RegisterSimulateSendCallback<Handshake>([this](Handshake msg) {
        InputMessage inoutMsg {x:10, y:20};
        Esp::SimulateSend(msg);
        delay(1000);
     });
#endif
}

void BrainApp::Update()
{
    Esp::Update();

    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::BROADCASTING)
    {
        LogInfo("Sending connection request..., thread: ", xPortGetCoreID());
        ConnectionRequest request(Esp::GetMacAddress());
        Esp::Send(BROADCAST_ADDRESS, request);
        delay(BRAIN_BROADCAST_DELAY);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        //add a timer here to return to BROADCASTING after a while + add broadcast peer again
        Esp::RemovePeer(BROADCAST_ADDRESS);
        LogInfo("Adding peer: ", controllerMac);
        Esp::AddPeer(controllerMac);
        LogInfo("Sending handshake message..., thread", xPortGetCoreID());
        Handshake handshake(Esp::GetMacAddress());
        Esp::Send(handshake);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        //remove timer
    }
}

ControllerApp::ControllerApp(const AppConfig &config)
    : state(ConnectionState::WAITING_FOR_CONNECTION), isComplete(false) {}

void ControllerApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

    Esp::RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        state = ConnectionState::SENDING_REQUEST;
        brainMac = msg.from;
        LogInfo("Received connection request from: ", brainMac);
     });
    Esp::RegisterReceiveCallback<Handshake>([this](Handshake msg) { 
        state = SENDING_HANDSHAKE;
        LogInfo("Received handshake message");
     });

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}}; 
        Handshake hand(otherMac);
        Esp::SimulateSend(hand);
    });
#endif
}

void ControllerApp::Update()
{ 
    Esp::Update();

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
        MacAddress receiverMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
        ConnectionRequest msg(receiverMac);
        Esp::SimulateSend(msg);
#endif
    }

    if (state == ConnectionState::SENDING_REQUEST)
    {
        LogInfo("Adding peer: ", brainMac);
        Esp::AddPeer(brainMac);
        LogInfo("Sending connection request.");
        ConnectionRequest request(Esp::GetMacAddress());
        Esp::Send(request);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
        return;
    }
    if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        LogInfo("Sending handshake to: ", brainMac);
        Handshake handshake(Esp::GetMacAddress());
        Esp::Send(handshake);
        state = ConnectionState::CONNECTED;
        return;
    }
    if (state == ConnectionState::CONNECTED && !isComplete)
    {
        LogInfo("Sending input message");
        InputMessage msg { x:10, y:20 };
        Esp::Send(msg);
        isComplete = true;
    }
}