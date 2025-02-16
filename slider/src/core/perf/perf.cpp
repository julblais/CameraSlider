#include "perf.h"

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#endif

using namespace Performance;

template<typename TTag, typename TSample, typename TValue>
Sampler<TTag, TSample, TValue>::Sampler()
{}

template<typename TTag, typename TSample, typename TValue>
Performance::Sampler<TTag, TSample, TValue>::~Sampler()
{}

template<typename TTag, typename TSample, typename TValue>
size_t Performance::Sampler<TTag, TSample, TValue>::printTo(Print& p) const
{
    auto size = p.print(TTag::Name);
    size += p.println("Toot");
    return size;
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::AddSample(const TSample& sample)
{}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::Reset()
{}

template<typename TTag, typename TSample, typename TValue>
TValue Sampler<TTag, TSample, TValue>::GetAverage()
{
    return TValue();
}

template<typename TTag, typename TSample, typename TValue>
unsigned int Sampler<TTag, TSample, TValue>::GetSampleCount()
{
    return 0;
}














MeasureTime::TimeSample::TimeSample(uint64_t* value, unsigned int* count)
    :m_Start(esp_timer_get_time()), m_Value(value), m_Count(count)
{}

MeasureTime::TimeSample::~TimeSample()
{
    auto end = esp_timer_get_time();
    (*m_Value) += end - m_Start;
    (*m_Count)++;
}

MeasureTime::MeasureTime()
    : m_Count(0), m_Value(0)
{}

MeasureTime::TimeSample MeasureTime::CreateSample()
{
    return TimeSample(&m_Value, &m_Count);
}

void MeasureTime::AddSample(uint64_t value)
{
    m_Count++;
    m_Value += value;
}

void MeasureTime::Reset()
{
    m_Count = 0;
    m_Value = 0;
}

uint64_t MeasureTime::GetMicrosecondsAverage()
{
    return m_Value / m_Count / 1000;
}

float MeasureTime::GetMillisecondsAverage()
{
    const float div = 1000.0f;
    return (float)m_Value / (float)m_Count / div;
}

unsigned int Performance::MeasureTime::GetSampleCount()
{
    return m_Count;
}

CpuTime::CpuTime()
{
    m_StartMicroseconds = esp_timer_get_time();
}

uint64_t CpuTime::GetValue()
{
    return (esp_timer_get_time() - m_StartMicroseconds) / 1000;
}

const char* CpuTime::GetUnit()
{
    return "ms";
}

Performance::CpuUsage::CpuUsage()
{}

uint8_t Performance::CpuUsage::GetValue()
{
    return 0;
}

const char* Performance::CpuUsage::GetUnit()
{
    return nullptr;
}
