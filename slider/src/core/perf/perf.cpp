#include "perf.h"

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#endif

#include <algorithm>

using namespace Performance;

template<typename TTag, typename TSample, typename TValue>
size_t Performance::Sampler<TTag, TSample, TValue>::printTo(Print& p) const
{
    auto size = p.print(TTag::Name);
    size += p.println("Toot");
    return size;
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::EndSample()
{
    auto val = m_Sample.GetValue();
    m_Count++;
    m_Sum += val;
    m_Max = std::max(m_Max, val);
    m_Min = std::min(m_Min, val);
    m_Sample = TSample();
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::Reset()
{
    m_Count = {};
    m_Max = {};
    m_Min = {};
}

template<typename TTag, typename TSample, typename TValue>
TValue Sampler<TTag, TSample, TValue>::GetAverage()
{
    return m_Sum / m_Count;
}

template<typename TTag, typename TSample, typename TValue>
unsigned int Sampler<TTag, TSample, TValue>::GetSampleCount()
{
    return m_Count;
}



void CpuTime::Start()
{
    m_StartMicroseconds = esp_timer_get_time();
}

uint64_t CpuTime::GetValue() const
{
    return (esp_timer_get_time() - m_StartMicroseconds) / 1000;
}

const char* CpuTime::GetUnit() const
{
    return "ms";
}

void CpuUsage::Start()
{

}

uint8_t Performance::CpuUsage::GetValue() const
{
    return 0;
}

const char* Performance::CpuUsage::GetUnit() const
{
    return "%";
}