#ifndef IS_SIMULATOR

#include "esp.h"
#include "src/debug.h"
#include "src/core/network/address.h"

#include <functional>

#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

using namespace Net;
using namespace Core;

Esp::SendCallback s_SendCallback {};
Esp::ReceiveCallback s_ReceiveCallback {};

void OnReceive(const uint8_t* mac_addr, const uint8_t* data, size_t length)
{
    LogDebug("Received message from: ", MacAddress(mac_addr));
    if (s_ReceiveCallback)
        s_ReceiveCallback(data, length);
}

void OnSend(const uint8_t* mac_addr, esp_now_send_status_t status)
{
    MacAddress address(mac_addr);
    LogDebug("Sending message to: ", address);
    if (s_SendCallback)
        s_SendCallback(address, status == ESP_NOW_SEND_SUCCESS);
}

bool Esp::Init()
{
    LogInfo("Init network...");

    LogDebug("Init wifi...");
    WiFi.mode(WIFI_STA);

    LogDebug("Init espnow...");
    auto result = esp_now_init();
    if (result != ESP_OK)
    {
        LogError("Error initializing ESP-NOW: ", esp_err_to_name(result));
        return false;
    }

    LogDebug("Registering receive callback...");
    result = esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive));
    if (result != ESP_OK)
    {
        LogError("Failed to register receive callback: ", esp_err_to_name(result));
        return false;
    }

    LogDebug("Registering receive callback...");
    result = esp_now_register_send_cb(esp_now_send_cb_t(OnSend));
    if (result != ESP_OK)
    {
        LogError("Failed to register send callback: ", esp_err_to_name(result));
        return false;
    }

    return true;
}

MacAddress Esp::GetMacAddress()
{
    std::array<uint8_t, 6> address;
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, address.data());
    if (ret == ESP_OK)
    {
        MacAddress mac(address);
        LogDebug("Get mac address: ", mac);
        return mac;
    }
    else
    {
        LogError("Failed to read mac address: ", esp_err_to_name(ret));
        return INVALID_ADDRESS;
    }
}

bool Esp::AddPeer(const MacAddress& address)
{
    esp_now_peer_info_t peerInfo { channel:0, encrypt : false };
    address.CopyTo(peerInfo.peer_addr);

    LogDebug("Trying to add peer: ", address);

    auto result = esp_now_add_peer(&peerInfo);
    if (result != ESP_OK)
    {
        LogError("Failed to add peer: ", esp_err_to_name(result));
        return false;
    }
    else
    {
        LogDebug("Added peer: ", address);
        return true;
    }
}

bool Esp::RemovePeer(const MacAddress& address)
{
    LogDebug("Trying to remove peer: ", address);

    auto result = esp_now_del_peer(address.Data());
    if (result != ESP_OK)
    {
        LogError("Failed to remove peer: ", esp_err_to_name(result));
        return false;
    }
    else
    {
        LogDebug("Removed peer: ", address);
        return true;
    }
}

void Esp::RegisterReceiveCb(const ReceiveCallback& callback)
{
    s_ReceiveCallback = callback;
}

void Esp::RegisterSendCallback(const Esp::SendCallback& callback)
{
    s_SendCallback = callback;
}

bool Esp::Send(const uint8_t* address, const uint8_t* data, size_t len)
{
    esp_err_t result = esp_now_send(address, data, len);
    if (result == ESP_OK)
    {
        LogDebug("Message sent.");
        return true;
    }
    else
    {
        LogError("Error sending message: ", esp_err_to_name(result));
        return false;
    }
}

#endif