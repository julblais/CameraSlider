#include "wifiComponent.h"
#include "wifiModule.h"

void Net::WifiComponent::Setup()
{
    WifiModule::GetInstance().Setup();
}

void Net::WifiComponent::Update()
{
    WifiModule::GetInstance().Update();
}
