#include "perf.h"
#include "src/debug.h"

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#endif

#include <algorithm>

using namespace Performance;

template<typename TTag, typename TSample, typename TValue>
Sampler<TTag, TSample, TValue>::Sampler(
    const unsigned int skip, const unsigned int logFrequency)
    : m_Sample(),
    m_Data(),
    m_SkipCount(skip),
    m_LogFreqCount(logFrequency)
{}

template<typename TTag, typename TSample, typename TValue>
size_t Sampler<TTag, TSample, TValue>::printTo(Print& p) const
{
    auto size = p.printf("%s(%s)\tcurrent ", TTag::Name, TSample::Unit);
    size += p.print(m_Data.last);
    size += p.print("\tmean ");
    size += p.print(GetAverage());
    size += p.print("\tmax ");
    size += p.print(m_Data.max);
    size += p.print("\tmin ");
    size += p.print(m_Data.min);
    return size;
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::End()
{
    auto val = m_Sample.GetValue();
    if (m_Data.skip++ < m_SkipCount)
    {
        m_Sample = TSample();
        return;
    }

    m_Data.count++;
    m_Data.last = val;
    m_Data.sum += val;
    m_Data.max = std::max(m_Data.max, val);
    if (m_Data.count == 1)
        m_Data.min = val;
    else
        m_Data.min = std::min(m_Data.min, val);
    m_Sample = TSample();

    if (m_Data.freq++ >= m_LogFreqCount)
    {
        LogPerf(*this);
        Reset();
    }
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::Reset()
{
    m_Data = {};
}

template<typename TTag, typename TSample, typename TValue>
TValue Sampler<TTag, TSample, TValue>::GetAverage() const
{
    if (m_Data.count == 0)
        return 0;
    return m_Data.sum / m_Data.count;
}

template<typename TTag, typename TSample, typename TValue>
unsigned int Sampler<TTag, TSample, TValue>::GetSampleCount()
{
    return m_Data.count;
}



void CpuTime::Start()
{
    m_StartMicroseconds = esp_timer_get_time();
}

uint64_t CpuTime::GetValue() const
{
    return (esp_timer_get_time() - m_StartMicroseconds);
}

void CpuUsage::Start()
{}

uint8_t CpuUsage::GetValue() const
{
    return 0;
}