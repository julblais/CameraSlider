#include "perf.h"

#ifdef ESP_32
#include "esp_timer.h"
#endif

using namespace Performance;

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
