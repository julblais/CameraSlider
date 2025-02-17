#include "cpuTime.h"

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#endif

using namespace Performance;

void CpuTime::Start()
{
    #ifdef ARDUINO_ARCH_ESP32
    m_StartMicroseconds = esp_timer_get_time();
    #else
    m_StartMicroseconds = micros();
    #endif
}

uint64_t CpuTime::GetValue() const
{
    #ifdef ARDUINO_ARCH_ESP32
    return (esp_timer_get_time() - m_StartMicroseconds);
    #else
    return (micros() - m_StartMicroseconds);
    #endif
}