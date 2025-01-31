#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"

#include <WiFi.h>
#include <esp_wifi.h>
//#include "ESPNowW.h"
//#include <WifiEspNowBroadcast.h>

const uint8_t master_mac[6] = { 0x94, 0x54, 0xc5, 0x63, 0x0a, 0xec };
const uint8_t slave_mac[6] = { 0x5c, 0x01, 0x3b, 0x68, 0xb1, 0x0c};

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

NetApp::NetApp(const AppConfig &config)
{
}

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");

    WiFi.mode(WIFI_STA);
    WiFi.begin();

    uint8_t* mac = readMacAddress();
    isMaster = isMacAddress(master_mac, mac);
    LogInfo("isMaster: ", isMaster);

    delete mac;
}

void NetApp::Update()
{
}
