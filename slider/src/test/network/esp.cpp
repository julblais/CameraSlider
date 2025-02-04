#include "esp.h"
#include "src/debug.h"

#include <array>
#include <functional>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>

MacAddress::MacAddress(std::array<uint8_t, 6> address)
    : Address(address) {}

MacAddress::MacAddress(const uint8_t* address) : Address()
{
    std::copy(address, address + 6, m_Address.begin());
}

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
        return INVALID;
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

static Esp::ReceiveCallback rcb;
static Esp::SendCallback scb;

void OnReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    MacAddress from(mac_addr);
    LogDebug("Received message from: ", from);
    rcb(from, data, data_len);
}

void OnSend(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    MacAddress to(mac_addr);
    scb(to, status == ESP_NOW_SEND_SUCCESS);
}

void Esp::RegisterReceiveCallback(ReceiveCallback callback)
{
    rcb = callback;
    auto res = esp_now_register_recv_cb(esp_now_recv_cb_t(OnReceive));
    if (res != ESP_OK)
    {
        LogError("Failed to register receive callback: ", esp_err_to_name(res));
    }
}

void Esp::RegisterSendCallback(SendCallback callback)
{
    scb = callback;
    auto res = esp_now_register_send_cb(esp_now_send_cb_t(OnSend));
    if (res != ESP_OK)
    {
        LogError("Failed to register send callback: ", esp_err_to_name(res));
    }
}