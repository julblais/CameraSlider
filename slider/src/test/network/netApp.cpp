#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
//#include <WifiEspNowBroadcast.h>

const uint8_t receiver_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
const uint8_t sender_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};
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

NetApp::NetApp(const AppConfig &config): isConnected(false) {}

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
    
    LogInfo("Received message: ", myData.x, " ", myData.y);
    if (myData.type == Type::COORDS && myData.x == msg.x && myData.y == msg.y)
    {
        receivedMsg = true;
    }
}

void NetApp::UpdateReceiver()
{
    if (isConnected)
        return;
    
    LogInfo("Trying to connect to sender");
    esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceiveMessage));

    LogInfo("Now connected to sender!");
    isConnected = true;
}

void NetApp::UpdateSender()
{
    if (!isConnected)
    {
        LogInfo("Trying to connect ");
        esp_now_register_send_cb(OnSendData);

        esp_now_peer_info_t peerInfo {};
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        memcpy(peerInfo.peer_addr, uni_mac, 6);
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            LogError("Failed to add peer");
            delay(3000);
            return;
        }
        LogInfo("Now connected to receiver!");
        isConnected = true;
    }

    if (isConnected && !msgSent)
    {
        LogInfo("Trying to send message: ");
        esp_err_t result = esp_now_send(uni_mac, (uint8_t *) &msg, sizeof(Message));
        
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

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");

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

    delete[] mac;
}

void NetApp::Update()
{
    if (isConnected && (receivedMsg || msgSent))
        digitalWrite(Led_Pin, LOW);
    else
        digitalWrite(Led_Pin, HIGH);

    if (isReceiver)
        UpdateReceiver();
    else
        UpdateSender();
}
