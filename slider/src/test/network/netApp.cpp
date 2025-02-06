#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"
#include "messageHandler.h"
#include "address.h"

#include "esp.h"

using namespace Slider;
using namespace Network;

#define Led_Pin 17

//const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};

struct ConnectionRequest
{
    uint8_t mac[6];
};

struct HandshakeComplete {};

struct InputMessage
{
    int x;
    int y;
};

REGISTER_MESSAGE_TYPE(ConnectionRequest, 1);
REGISTER_MESSAGE_TYPE(InputMessage, 2);
REGISTER_MESSAGE_TYPE(HandshakeComplete, 3);

BrainApp::BrainApp(const AppConfig &config)
    : isCompleted(false), isConnected(false), hasSentHandshake(false) {}

void BrainApp::Setup()
{
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

    Esp::RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) { 
        isConnected = true;
        controllerMac = msg.mac;
     });
    Esp::RegisterReceiveCallback<InputMessage>([this](InputMessage msg) { 
        isCompleted = true;
        LogInfo("Received message x:", msg.x, " y:", msg.y);
    });
    Esp::AddPeer(BROADCAST_ADDRESS);

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) {
        MacAddress otherMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}}; 
        ConnectionRequest omsg;
        otherMac.CopyTo(&omsg.mac[0]);
        Esp::SimulateSend(omsg);
     });
     Esp::RegisterSimulateSendCallback<HandshakeComplete>([this](HandshakeComplete msg) {
        InputMessage msg3 {x:10, y:20};
        Esp::SimulateSend(msg3);
        delay(1000);
     });
#endif
}

void BrainApp::Update()
{ 
    if (isConnected && isCompleted)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (!isConnected)
    {
        LogInfo("Sending connection request...");
        ConnectionRequest msg;
        MacAddress mac = Esp::GetMacAddress();
        mac.CopyTo(&msg.mac[0]);
        Esp::Send(BROADCAST_ADDRESS, msg);
        delay(2000);
        return;
    }

    if (!hasSentHandshake)
    {
        Esp::RemovePeer(BROADCAST_ADDRESS);
        LogInfo("Adding peer: ", controllerMac);
        Esp::AddPeer(controllerMac);
        LogInfo("Sending handshake message...");
        HandshakeComplete msg;
        Esp::Send(msg);
        hasSentHandshake = true;
        return;
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
    Esp::RegisterReceiveCallback<HandshakeComplete>([this](HandshakeComplete msg) { 
        hasHandshake = true;
     });

#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<ConnectionRequest>([this](ConnectionRequest msg) { 
        HandshakeComplete rmsg;
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