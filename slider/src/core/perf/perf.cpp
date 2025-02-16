#include "perf.h"

#ifdef ARDUINO_ARCH_ESP32 
#include "esp_timer.h"
#endif

using namespace Performance;

template</*const Tag& tag,*/ typename TSample, typename TValue>
Sampler< TSample, TValue>::Sampler()
{}

template</*const Tag& tag,*/ typename TSample, typename TValue>
void Sampler< TSample, TValue>::AddSample(const TSample& sample)
{}

template</*const Tag& tag,*/ typename TSample, typename TValue>
void Sampler< TSample, TValue>::Reset()
{}

template</*const Tag& tag,*/ typename TSample, typename TValue>
TValue Sampler<TSample, TValue>::GetAverage()
{
    return TValue();
}

template</*const Tag& tag,*/ typename TSample, typename TValue>
unsigned int Sampler<TSample, TValue>::GetSampleCount()
{
    return 0;
}

template</*const Tag& tag,*/ typename TSample, typename TValue>
void Sampler<TSample, TValue>::PrintTo(Print& printer)
{}



/*
template<const Tag& tag, typename TSample, typename TValue>
Sampler<tag, TSample, TValue>::Sampler()
{}

template<const Tag& tag, typename TSample, typename TValue>
void Sampler<tag, TSample, TValue>::AddSample(const TSample& sample)
{}

template<const Tag& tag, typename TSample, typename TValue>
void Sampler<tag, TSample, TValue>::Reset()
{}

template<const Tag& tag, typename TSample, typename TValue>
TValue Sampler<tag, TSample, TValue>::GetAverage()
{
    return TValue();
}

template<const Tag& tag, typename TSample, typename TValue>
unsigned int Sampler<tag, TSample, TValue>::GetSampleCount()
{
    return 0;
}

template<const Tag& tag, typename TSample, typename TValue>
void Sampler<tag, TSample, TValue>::PrintTo(Print& printer)
{}

*/
















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
