#include "wifi.h"

#ifndef IS_SIMULATOR

using namespace Network;

#include "test/network/esp.h"

void Wifi::Setup()
{
    Network::Esp::Init();
}

void Wifi::Update()
{
}

#else

void Wifi::Setup()
{
}

void Wifi::Update()
{
}

#endif
