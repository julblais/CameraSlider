#include "perf.h"

#if LOG_PERFORMANCE

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#include "esp_system.h"
#endif

using namespace Performance;

Performance::CpuTimeSampler CpuSampler = (200u);
Performance::MemoryFreeSampler MemorySampler = (200u);

void CpuTime::Setup() {}

uint64_t CpuTime::GetValue() const
{
    #ifdef ARDUINO_ARCH_ESP32
    return esp_timer_get_time();
    #else
    return micros();
    #endif
}

void Performance::MemoryFreeSize::Setup() {}

int64_t Performance::MemoryFreeSize::GetValue() const
{
    auto eap = esp_get_free_heap_size() / 1000;
    return eap;
}

#endif //LOG_PERFORMANCE
