#include "cpuUsage.h"

using namespace Performance;

/*
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"

#if defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_240)
static const uint64_t MaxIdleCalls = 1855000;
#elif defined(CONFIG_ESP32_DEFAULT_CPU_FREQ_160)
static const uint64_t MaxIdleCalls = 1233100;
#else
#error "Unsupported CPU frequency"
#endif

static uint64_t idleCalls = 0;

static bool idle_task()
{
    idleCalls++;
    return false;
}

uint8_t CpuUsage::GetValue() const
{
    float idle = (float)idleCalls;
    idleCalls = 0;
    auto percentIdle = (idle * 100.0f) / MaxIdleCalls;
    return 100 - std::ceil(percentIdle);
}

void RegisterIdleHook()
{
    char* toto = new char[1024];
    vTaskGetRunTimeStats(toto);
    auto coreId = xPortGetCoreID();
    auto result = esp_register_freertos_idle_hook(idle_task);
    LogPerf("Adding idle hook for core ", coreId);
    if (result != ESP_OK)
        LogError("Error initializing cpu time sampler: ", esp_err_to_name(result));
}

void CpuUsage::Start()
{
    static bool isInit = false;
    if (!isInit)
    {
        isInit = true;
        RegisterIdleHook();
    }
}
*/
