#ifndef BROADCAST_H
#define BROADCAST_H

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

class broadcast
{
private:
    /* data */
public:
    void setup(/* args */);
};

void broadcast::setup(/* args */)
{
    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_MODE_STA);
  //WiFi.disconnect();

  Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
  readMacAddress();
    //WiFi.persistent(false);
    //bool ok = WifiEspNowBroadcast.begin("ESPNOW", 3);
}


#endif