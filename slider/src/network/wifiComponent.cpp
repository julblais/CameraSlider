#include "wifiComponent.h"
#include "wifiModule.h"

#ifdef USE_NETWORK

void Net::WifiComponent::Setup()
{
    WifiModule::GetInstance().Setup();
}

void Net::WifiComponent::Update()
{
    WifiModule::GetInstance().Update();
}

#endif
