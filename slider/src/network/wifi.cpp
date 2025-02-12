#include "wifi.h"

using namespace Net;

#ifndef IS_SIMULATOR

#include "src/test/network/esp.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "src/test/network/messageHandler.h"

void WifiComponent::Setup()
{
    Esp::Init();

    LogDebug("Registering receive callback...");
    MessageHandler handler;
    auto f2 = std::bind(&WifiComponent::OnReceive, this);
    auto result = esp_now_register_recv_cb(esp_now_recv_cb_t(f2));
    if (result != ESP_OK)
    {
        LogError("Failed to register receive callback: ", esp_err_to_name(result));
        return false;
    }
}

void WifiComponent::Update()
{
    Esp::Update();
}

#else

void WifiComponent::OnReceive(const uint8_t *mac_addr, const uint8_t *data, size_t length)
{
}

void WifiComponent::Setup()
{
}

void WifiComponent::Update()
{
}

#endif
