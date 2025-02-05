#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"
#include "messageHandler.h"

#ifndef IS_SIMULATOR
    #include "esp.h"
#endif
#ifdef IS_SIMULATOR
    #include "esp_sim.h"
#endif

const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};
const uint8_t uni_mac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/////////////////////////////////

using namespace Slider;
using namespace Network;

#define Led_Pin 17


REGISTER_MESSAGE_TYPE(HandshakeMessage, 1);
REGISTER_MESSAGE_TYPE(InputMessage, 2);

bool receivedMsg = false;
bool msgSent = false;
bool isDiscovered = false;
bool isConn = false;

NetApp::NetApp(const AppConfig &config) {}

void NetApp::UpdateReceiver()
{
}

void NetApp::UpdateSender()
{
}

void NetApp::Sender__ReceiveMsg(HandshakeMessage message)
{

}

void NetApp::Sender__ReceiveMsg(InputMessage message)
{

}

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");
    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);
    
    LogInfo("Init espnow...");
    Esp::Init();

#ifdef IS_RECEIVER
    //handler.AddCallback<Message>(OnSenderReceiveData);
#endif
#ifdef IS_SENDER
    Esp::RegisterReceiveCallback<HandshakeMessage>([this](HandshakeMessage msg) { this->Sender__ReceiveMsg(msg); });
    Esp::RegisterReceiveCallback<InputMessage>([this](InputMessage msg) { this->Sender__ReceiveMsg(msg); });
#endif
}

void NetApp::Update()
{
    if (isConn && (receivedMsg || msgSent))
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (isReceiver)
        UpdateReceiver();
    else
        UpdateSender();
}
