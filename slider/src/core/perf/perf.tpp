#include "perf.h"
#include "src/debug.h"

#include <algorithm>

using namespace Performance;

template<typename TTag, typename TSample, typename TValue>
Sampler<TTag, TSample, TValue>::Sampler(const unsigned int logFrequency)
    : m_Sample(), m_LogFreqCount(logFrequency)
{}

template<typename TTag, typename TSample, typename TValue>
size_t Sampler<TTag, TSample, TValue>::printTo(Print& p) const
{/*
    auto size = p.printf("%s:%s(%s)\tmean:", m_Name, TTag::Name, TSample::Unit);
    size += p.print(GetAverage());
    size += p.print("\tmax:");
    size += p.print(m_Data.max);
    size += p.print("\tmin:");
    size += p.print(m_Data.min);
    return size;
    */
   return 0;
}

template<typename TTag, typename TSample, typename TValue>
void Performance::Sampler<TTag, TSample, TValue>::Start()
{
    m_Sample.Setup();
}

template<typename TTag, typename TSample, typename TValue>
void Performance::Sampler<TTag, TSample, TValue>::BeginSample(const char* tag)
{
    m_Stack.push({ m_Sample.GetValue(), tag });
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::EndSample()
{
    auto endValue = m_Sample.GetValue();
    auto stackData = m_Stack.top();
    auto value = m_Sample.GetValue() - stackData.value;
    m_Stack.pop();

    auto found = m_Values.find(stackData.name);
    Data data;
    if (found != m_Values.end())
        data = found->second;

    data.count++;
    data.sum += value;
    data.max = std::max(data.max, value);
    if (data.count == 1)
        data.min = value;
    else
        data.min = std::min(data.min, value);
    m_Values[stackData.name] = data;
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::End()
{
    m_Stack = std::stack<StackData>();
    m_Values.clear();
    freq = 0;
    if (freq++ >= m_LogFreqCount)
    {
        LogPerf(*this);
        //Reset();
    }
}