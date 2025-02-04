#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

#include "esp.h"
//#include <WifiEspNowBroadcast.h>

const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
//const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};
const uint8_t uni_mac[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t* readMacAddress(){
  uint8_t* baseMac = new uint8_t[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
    return baseMac;
  } else {
    LogError("Failed to read MAC address");
  }
}

bool isMacAddress(const uint8_t* const mac, const uint8_t* const candidate){
    for(int i = 0; i < 6; i++) 
    {
        if(mac[i] != candidate[i]){
            return false;
        }
    }
    return true;
}

/////////////////////////////////

using namespace Slider;
using namespace Network;

#define Led_Pin 17

enum Type { HANDSHAKE, COORDS };

struct Message {
    Type type;
    int x;
    int y;
    uint8_t mac[6];
};

const Message msg = {type:Type::COORDS, x:11, y:22};
bool receivedMsg = false;
bool msgSent = false;
bool isDiscovered = false;
bool isConn = false;
MessageHandler handler;

NetApp::NetApp(const AppConfig &config) {}

void OnSendData(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  char macStr[18];
  LogInfo("Packet to: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  LogInfo(macStr, " send status:\t", status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  msgSent = status == ESP_NOW_SEND_SUCCESS;
}

void OnReceiveMessage(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
    Message myData;
    memcpy(&myData, incomingData, sizeof(myData));
    
    if (myData.type == Type::COORDS && myData.x == msg.x && myData.y == msg.y)
    {
        LogInfo("Received message: ", myData.x, " ", myData.y);
        receivedMsg = true;
    }
    else if (myData.type == Type::HANDSHAKE)
    {
        char macStr[18];
        snprintf(macStr, sizeof(myData.mac), "%02x:%02x:%02x:%02x:%02x:%02x",
           myData.mac[0], myData.mac[1], myData.mac[2], myData.mac[3], myData.mac[4], myData.mac[5]);
           
        LogInfo("Discovered sender: ", macStr);
        esp_now_peer_info_t peerInfo {};
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        memcpy(peerInfo.peer_addr, myData.mac, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            LogError("Failed to add peer");
            delay(3000);
            return;
        }
        LogInfo("Now connected to sender!");

        uint8_t* macAddress = readMacAddress();
        Message msg = {type:Type::HANDSHAKE };
        memcpy(&msg.mac, &macAddress[0], sizeof(msg.mac));
        esp_err_t result = esp_now_send(myData.mac, (uint8_t *) &msg, sizeof(Message));
        if (result == ESP_OK) 
        {
            LogInfo("Handshake request sent!");
        }
        else 
        {
            LogInfo("Error sending handshake: ", esp_err_to_name(result));
        }

        isDiscovered = true;
    }
}

void NetApp::UpdateReceiver()
{
    if (!isDiscovered)
    {
        //LogInfo("Waiting for sender...");
        return;
    }

    if (isConn)
        return;
    

    //LogInfo("Now connected to sender!");
    isConn = true;
}

void NetApp::UpdateSender()
{
    if (!isDiscovered)
    {
        LogInfo("Trying to discover receiver...");
        uint8_t* macAddress = readMacAddress();

     

        Message msg = {type:Type::HANDSHAKE, x:0, y:0};
        memcpy(&msg.mac, &macAddress[0], sizeof(msg.mac));
        //send own mac address to receiver
        esp_err_t result = esp_now_send(uni_mac, (uint8_t *) &msg, sizeof(Message));
        if (result == ESP_OK) 
        {
            LogInfo("Handshake request sent!");
            delay(1500);
        }
        else 
        {
            LogInfo("Error sending handshake: ", esp_err_to_name(result));
        }
    }
    else if (!isConn)
    {
        LogInfo("Waiting for receiver connection...");
    }
    else if (isConn && !msgSent)
    {
        LogInfo("Trying to send message: ");
        esp_err_t result = esp_now_send(0, (uint8_t *) &msg, sizeof(Message));
        
        if (result == ESP_OK) 
        {
            LogInfo("Msg request sent!");
        }
        else 
        {
            LogInfo("Error sending the data");
        }
        delay(1000);
    }
}

void OnSenderReceiveData(const uint8_t * mac, const uint8_t *incomingData, int len)
{
    Message myData;
    memcpy(&myData, incomingData, sizeof(myData));

    if (myData.type == Type::HANDSHAKE)
    {
        isDiscovered = true;
        LogInfo("Discovered receiver! now adding peer");
        char macStr[18];
        snprintf(macStr, sizeof(myData.mac), "%02x:%02x:%02x:%02x:%02x:%02x",
           myData.mac[0], myData.mac[1], myData.mac[2], myData.mac[3], myData.mac[4], myData.mac[5]);
           
        LogInfo("Discovered receiver: ", macStr);
        esp_now_peer_info_t peerInfo {};
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        memcpy(peerInfo.peer_addr, myData.mac, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            LogError("Failed to add peer");
            delay(3000);
            return;
        }
        isConn = true;
        msgSent = false;
        LogInfo("Peer added!");
    }
}

REGISTER_TYPE_ID(Network::MessageExample, 1);
REGISTER_TYPE_ID(Network::CustomMessage, 2);

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");
    Network::MessageExample example;
    example.id = MessageTypeId<MessageExample>::id;
    example.x = 10;
    example.y = 20;

    Network::CustomMessage custom;
    custom.id = MessageTypeId<CustomMessage>::id;
    custom.x = 10;
    custom.y = 20;
    custom.toto = true;
    LogInfo("Example: ", MessageTypeId<MessageExample>::id, " ", example.x, " ", example.y);

    handler.AddCb<MessageExample>([](MessageExample msg){
        LogInfo("Handled message example: ", msg.x, " ", msg.y);
    });
    handler.AddCb<CustomMessage>([](CustomMessage msg){
        LogInfo("Handled custom message: ", msg.x, " ", msg.y, " ", msg.toto);
    });

    handler.Invoke((uint8_t*)&example);
    handler.Invoke((uint8_t*)&custom);

    pinMode(Led_Pin, OUTPUT);
    digitalWrite(Led_Pin, HIGH);

    WiFi.mode(WIFI_STA);
    
    LogInfo("Init espnow...");

    if (esp_now_init() != ESP_OK)
        LogError("Error initializing ESP-NOW");

    LogInfo("Checking mac address....");
    uint8_t* mac = readMacAddress();
    isReceiver = isMacAddress(receiver_mac, mac);
    LogInfo("Is receiver: ", isReceiver);

    if (!isReceiver)
    {
        esp_now_register_recv_cb(esp_now_recv_cb_t(OnSenderReceiveData));
        esp_now_register_send_cb(OnSendData);   
        esp_now_peer_info_t peerInfo {};
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        memcpy(peerInfo.peer_addr, uni_mac, 6);
        auto res = esp_now_add_peer(&peerInfo);
        if (res != ESP_OK){
            LogError("Failed to add peer: ", esp_err_to_name(res));
            delay(3000);
            return;
        }
    }
    else{
        
        LogInfo("Adding callback");
        esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceiveMessage));
    }

    delete[] mac;
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
