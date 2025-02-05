#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"
#include "messageHandler.h"
#include "address.h"

#ifndef IS_SIMULATOR
    #include "esp.h"
#endif
#ifdef IS_SIMULATOR
    #include "esp_sim.h"
#endif

//const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};
//const uint8_t uni_mac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/////////////////////////////////

using namespace Slider;
using namespace Network;

#define Led_Pin 17


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

NetApp::NetApp(const AppConfig &config): isCompleted(false), isConnected(false) {}

void NetApp::UpdateReceiver()
{
    if (!isConnected)
    {
        ConnectionRequest msg;
        MacAddress mac = Esp::GetMacAddress();
        mac.CopyTo(&msg.mac[0]);
        Esp::Send(msg);
        return;
    }

    if (!hasHandshake)
    {
        HandshakeComplete msg2;
        Esp::Send(msg2);
        return;
    }
}

void NetApp::UpdateController()
{
    if (!isConnected)
    {
#ifdef IS_SIMULATOR
        ConnectionRequest msg;
        MacAddress receiverMac{{0x11, 0x22, 0x33, 0x44, 0x55, 0x66}};
        receiverMac.CopyTo(&msg.mac[0]);
        Esp::SimulateSend(msg);
#endif
        return;
    }

    if (!hasHandshake)
    {
        auto mac = Esp::GetMacAddress();
        ConnectionRequest msg;
        mac.CopyTo(&msg.mac[0]);
        Esp::Send(msg);
        delay(1000);
        return;
    }

    if (!isCompleted)
    {
        InputMessage msg { x:10, y:20 };
        Esp::Send(msg);
    }
}

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

#ifdef IS_RECEIVER
    Esp::RegisterReceiveCallback<ConnectionRequest>([this](ConnectionRequest msg) { 
        isConnected = true;
        otherMac = msg.mac;
     });
    Esp::RegisterReceiveCallback<InputMessage>([this](InputMessage msg) { 
        isCompleted = true;
        LogInfo("Received message x:", msg.x, " y:", msg.y);
    });
    Esp::RegisterSendCallback([this](const Network::MacAddress& a, bool ok) { 
        hasHandshake = isConnected && ok;
        isCompleted = isConnected && ok;
     });
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
#endif

#ifdef IS_SENDER
    Esp::RegisterReceiveCallback<HandshakeMessage>([this](HandshakeMessage msg) { 
        isConnected = true;
        otherMac = msg.mac;
     });
    Esp::RegisterReceiveCallback<HandshakeComplete>([this](HandshakeComplete msg) { 
        hasHandshake = true;
     });
    Esp::RegisterSendCallback([this](const Network::MacAddress& a, bool ok) { 
        isCompleted = isConnected && hasHandshake && ok;
     });
#ifdef IS_SIMULATOR
    Esp::RegisterSimulateSendCallback<HandshakeMessage>([this](HandshakeMessage msg) { 
        HandshakeComplete rmsg;
        Esp::SimulateSend(rmsg);
     });
#endif
#endif
}

void NetApp::Update()
{
    if (isConnected && isCompleted)
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

#ifdef IS_RECEIVER
        UpdateReceiver();
#else
        UpdateController();
#endif
}
