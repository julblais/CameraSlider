#include "esp.h"
#include "src/debug.h"
#include "address.h"

#include <functional>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

using namespace Network;

bool Esp::Init()
{
    LogDebug("Init wifi...");
    WiFi.mode(WIFI_STA);
    
    LogDebug("Init espnow...");
    auto result = esp_now_init();
    if (result != ESP_OK)
    {
        LogError("Error initializing ESP-NOW: ", esp_err_to_name(result));
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
        LogDebug("Found mac address: ", mac);
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
    esp_now_peer_info_t peerInfo {};
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
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
        LogInfo("Added peer: ", address);
        return true;
    }
}

bool Esp::RemovePeer(const MacAddress& address)
{
    esp_now_peer_info_t peerInfo {};
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
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
        LogInfo("Added peer: ", address);
        return true;
    }
}

void Network::Esp::Send(const uint8_t *data, size_t len)
{
    esp_err_t result = esp_now_send(nullptr, data, len);
    if (result == ESP_OK) 
    {
        LogDebug("Message sent!");
    }
    else 
    {
        LogInfo("Error sending message: ", esp_err_to_name(result));
    }
}

void Network::Esp::Send(const MacAddress &address, const uint8_t* data, size_t len)
{
    esp_err_t result = esp_now_send(address.Data(), data, len);
    if (result == ESP_OK) 
    {
        LogDebug("Message sent to:", address);
    }
    else 
    {
        LogInfo("Error sending message to: ", address, " - ", esp_err_to_name(result));
    }
}

static Esp::ReceiveCallback s_ReceiveCallback;
static Esp::SendCallback s_SendCallback;

void OnReceive(const uint8_t* mac_addr, const uint8_t *data, int data_len)
{
    MacAddress from(mac_addr);
    LogDebug("Received message from: ", from);
    if (s_ReceiveCallback)
        s_ReceiveCallback(from, data, data_len);
}

void OnSend(const uint8_t* mac_addr, esp_now_send_status_t status)
{
    MacAddress to(mac_addr);
    if (s_SendCallback)
        s_SendCallback(to, status == ESP_NOW_SEND_SUCCESS);
}

void Esp::RegisterReceiveCallback(ReceiveCallback callback)
{
    s_ReceiveCallback = callback;
    auto res = esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive));
    if (res != ESP_OK)
    {
        LogError("Failed to register receive callback: ", esp_err_to_name(res));
    }
}

void Esp::RegisterSendCallback(SendCallback callback)
{
    s_SendCallback = callback;
    auto res = esp_now_register_send_cb(esp_now_send_cb_t(OnSend));
    if (res != ESP_OK)
    {
        LogError("Failed to register send callback: ", esp_err_to_name(res));
    }
}
