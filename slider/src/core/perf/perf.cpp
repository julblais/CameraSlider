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
    m_Count(0),
    m_Skip(0),
    m_SkipCount(skip),
    m_Freq(0),
    m_LogFreqCount(logFrequency),
    m_Sum(), m_Max(), m_Min(), m_Last()
{}

template<typename TTag, typename TSample, typename TValue>
size_t Sampler<TTag, TSample, TValue>::printTo(Print& p) const
{
    auto size = p.printf("%s(%s)\tcurrent ", TTag::Name, TSample::Unit);
    size += p.print(m_Last);
    size += p.print("\tmean ");
    size += p.print(GetAverage());
    size += p.print("\tmax ");
    size += p.print(m_Max);
    size += p.print("\tmin ");
    size += p.print(m_Min);
    return size;
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::End()
{
    if (m_Skip++ < m_SkipCount)
    {
        m_Sample = TSample();
        return;
    }

    auto val = m_Sample.GetValue();
    m_Count++;
    m_Last = val;
    m_Sum += val;
    m_Max = std::max(m_Max, val);
    if (m_Count == 1)
        m_Min = val;
    else
        m_Min = std::min(m_Min, val);
    m_Sample = TSample();

    if (m_Freq++ >= m_LogFreqCount)
    {
        m_Freq = 0;
        LogPerf(*this);
        Reset();
    }
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::Reset()
{
    m_Count = {};
    m_Max = {};
    m_Min = {};
    m_Last = {};
    m_Sum = {};
    m_Skip = 0;
    m_Freq = 0;
}

template<typename TTag, typename TSample, typename TValue>
TValue Sampler<TTag, TSample, TValue>::GetAverage() const
{
    if (m_Count == 0)
        return 0;
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
    return (esp_timer_get_time() - m_StartMicroseconds);
}

void CpuUsage::Start()
{}

uint8_t CpuUsage::GetValue() const
{
    return 0;
}