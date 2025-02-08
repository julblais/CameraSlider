#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"
#include "messageHandler.h"
#include "address.h"

#include "esp.h"
using namespace Slider;
using namespace Network;

#define Led_Pin 17

/*
                 ┌─────────┐                              ┌────────────┐                 
                 │         │                              │            │                 
                 │ Slider  │                              │ Controller │                 
                 │         │                              │            │                 
                 └─────┬───┘                              └─────┬──────┘                 
                       │                                        │                        
         BROADCASTING  │                                        │  WAITING_FOR_CONNECTION
                       │      ConnectionRequest                 │                        
                       ├───────────────────────────────────────►│                        
                       │                                        │                        
                       │                   ConnectionRequest    │  SENDING_REQUEST       
                       │◄───────────────────────────────────────┤                        
                       │                                        │                        
    SENDING_HANDSHAKE  │                                        │  WAITING_FOR_HANDSHAKE 
                       │   Handshake                            │                        
                       ├───────────────────────────────────────►│                        
                       │                                        │                        
WAITING_FOR_HANDSHAKE  │                                        │  SENDING_HANDSHAKE     
                       │                           Handshake    │                        
                       │◄───────────────────────────────────────┤                        
                       │                                        │                        
            CONNECTED  │                                        │  CONNECTED             
 */

//const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};

struct ConnectionRequest
{
    uint8_t mac[6];
};

struct Handshake {};

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
        state = ConnectionState::SENDING_HANDSHAKE;
        controllerMac = msg.mac;
        LogInfo("Received connection msg, Thread: ", xPortGetCoreID());
    });
    Esp::RegisterReceiveCallback<Handshake>([this](Handshake msg) { 
        state = ConnectionState::CONNECTED;
        LogInfo("received handshake: Thread: ", xPortGetCoreID());
    });
    Esp::RegisterReceiveCallback<InputMessage>([this](InputMessage msg) { 
        isComplete = true;
        LogInfo("Received message x:", msg.x, " y:", msg.y);
        LogInfo("input msg Thread: ", xPortGetCoreID());
    });
    Esp::AddPeer(BROADCAST_ADDRESS);

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}}; 
        ConnectionRequest connectMsg;
        otherMac.CopyTo(&msg.mac[0]);
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
    if (isComplete)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (state == ConnectionState::BROADCASTING)
    {
        LogInfo("Sending connection request..., thread: ", xPortGetCoreID());
        ConnectionRequest msg;
        MacAddress mac = Esp::GetMacAddress();
        mac.CopyTo(&msg.mac[0]);
        Esp::Send(BROADCAST_ADDRESS, msg);
        delay(2000);
    }
    else if (state == ConnectionState::SENDING_HANDSHAKE)
    {
        //add a timer here to return to BROADCASTING after a while + add broadcast peer again
        Esp::RemovePeer(BROADCAST_ADDRESS);
        LogInfo("Adding peer: ", controllerMac);
        Esp::AddPeer(controllerMac);
        LogInfo("Sending handshake message..., thread", xPortGetCoreID());
        Handshake msg;
        Esp::Send(msg);
        state = ConnectionState::WAITING_FOR_HANDSHAKE;
    }
    else if (state == ConnectionState::CONNECTED)
    {
        //remove timer
    }
}

ControllerApp::ControllerApp(const AppConfig &config)
    : isCompleted(false), isConnected(false), hasHandshake(false), hasSentHandshake(false) {}

void ControllerApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

    Esp::RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) { 
        isConnected = true;
        brainMac = msg.mac;
     });
    Esp::RegisterReceiveCallback<Handshake>([this](Handshake msg) { 
        hasHandshake = true;
     });

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) { 
        Handshake rmsg;
        Esp::SimulateSend(rmsg);
     });
#endif
}

void ControllerApp::Update()
{ 
    if (isConnected && isCompleted)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (!isConnected)
    {
#ifdef IS_SIMULATOR
        ConnectionRequest msg;
        MacAddress receiverMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
        receiverMac.CopyTo(&msg.mac[0]);
        Esp::SimulateSend(msg);
#endif
        LogInfo("Waiting for connection request");
        delay(1000);
        return;
    }

    if (!hasSentHandshake)
    {
        LogInfo("Received connection request");
        LogInfo("Adding peer: ", brainMac);
        Esp::AddPeer(brainMac);
        auto mac = Esp::GetMacAddress();
        LogInfo("Sending connection request.");
        ConnectionRequest msg;
        mac.CopyTo(&msg.mac[0]);
        hasSentHandshake = true;
        Esp::Send(msg);
        return;
    }

    if (hasHandshake && !isCompleted)
    {
        LogInfo("Received handshake");
        LogInfo("Sending input message");
        InputMessage msg { x:10, y:20 };
        Esp::Send(msg);
        isCompleted = true;
    }
}