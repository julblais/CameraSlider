#include "netApp.h"
#include "src/app/appConfig.h"
#include "src/debug.h"


//#include <WifiEspNowBroadcast.h>

#include <WiFi.h>
#include <esp_wifi.h>
//#include "ESPNowW.h"

void readMacAddress(){
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

/////////////////////////////////

using namespace Slider;

NetApp::NetApp(const AppConfig &config)
{
}

void NetApp::Setup()
{    
    LogInfo("Setup NetApp");
}

void NetApp::Update()
{
}
