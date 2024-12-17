#include "utils/timer.h"

#include <esp32-hal-timer.h>

Timer::Timer() : m_AppTimeMs(0) {}

unsigned int Timer::GetTimeMs() 
{
    return millis() - m_AppTimeMs; 
}

void Timer::Start()
{
    m_AppTimeMs = millis(); 
}

void Timer::Stop()
{
    m_AppTimeMs = 0;
}
