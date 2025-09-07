#ifndef PERF_TPP
#define PERF_TPP

#include "perf.h"
#include "src/debug.h"
#include "src/core/utils/tableFormatter.h"

#include <algorithm>

using namespace Performance;

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
Sampler<TTag, TSample, TValue, IsAbsolute>::Sampler(const unsigned int logFrequency) :
    m_Sample(),
    m_LogFreqCount(logFrequency) {}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::Setup()
{
    m_Sample.Setup();
}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::BeginSample(const char* tag)
{
    m_Stack.push({ m_Sample.GetValue(), tag, TValue() });
}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::EndSample()
{
    TValue endValue = m_Sample.GetValue();
    auto stackData = m_Stack.top();
    m_Stack.pop();

    auto found = m_Values.find(stackData.tag);
    Data data = {};
    if (found != m_Values.end())
        data = found->second;

    auto value = endValue;
    if (!IsAbsolute)
        value -= stackData.value + stackData.bias;
    data.count++;
    data.current = value;
    data.sum += value;
    data.max = std::max(data.max, value);
    data.parent = m_Stack.empty() ? nullptr : m_Stack.top().tag;
    if (data.count == 1)
        data.min = value;
    else
        data.min = std::min(data.min, value);

    m_Values[stackData.tag] = data;
    if (!m_Stack.empty())
        m_Stack.top().bias += m_Sample.GetValue() - endValue + stackData.bias;
}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::AddData(const DataPair& pair, Core::TableFormatter& table,
                                                         int indent) const
{
    auto data = pair.second;
    auto indentStr = std::string(indent, '-');
    table.AddRow({
        indentStr.append(pair.first),
        std::to_string(data.sum / data.count),
        std::to_string(data.max),
        std::to_string(data.min),
        std::to_string(data.current)
    });
    for (auto& child: m_Values)
        if (child.second.parent == pair.first)
            AddData(child, table, indent + 1);
}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::Log()
{
    Core::TableFormatter table(5, m_Values.size(), 1);
    table.AddRow({ TTag::Name, "Mean", "Max", "Min", "Last" });

    for (auto& pair: m_Values)
        if (pair.second.parent == nullptr)
            AddData(pair, table, 0);
    LogPerf(table);
}

template<typename TTag, typename TSample, typename TValue, bool IsAbsolute>
void Sampler<TTag, TSample, TValue, IsAbsolute>::Finish()
{
    if (freq++ >= m_LogFreqCount)
    {
        Log();
        m_Stack = std::stack<StackData>();
        m_Values.clear();
        freq = 0;
    }
}
#endif // PERF_TPP
