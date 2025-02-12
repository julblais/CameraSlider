#include "wifi.h"

using namespace Net;

#ifndef IS_SIMULATOR

#include "src/test/network/esp.h"

void Wifi::Setup()
{
    Esp::Init();
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
