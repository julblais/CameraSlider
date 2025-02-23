#include "perf.h"
#include "src/debug.h"
#include "src/core/utils/tableFormatter.h"

#include <algorithm>

using namespace Performance;

static unsigned int m_IdGenerator = 0;

template<typename TTag, typename TSample, typename TValue>
Sampler<TTag, TSample, TValue>::Sampler(const unsigned int logFrequency)
    : m_Sample(), m_LogFreqCount(logFrequency)
{
    m_Sample.Setup();
}

template<typename TTag, typename TSample, typename TValue>
void Performance::Sampler<TTag, TSample, TValue>::Start()
{}

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

    auto found = m_Values.find(stackData.tag);
    Data data = {};
    if (found != m_Values.end())
        data = found->second;

    data.count++;
    data.sum += value;
    data.indent = m_Stack.size();
    data.max = std::max(data.max, value);
    data.parent = m_Stack.empty() ? nullptr : m_Stack.top().tag;
    if (data.count == 1)
        data.min = value;
    else
        data.min = std::min(data.min, value);

    m_Values[stackData.tag] = data;
/*
        //calculate another end value
        //then remove it from all elements of the stack.
        auto endValue2 = m_Sample.GetValue();
        for (auto& stackData : m_Stack)
        {
            stackData.value -= endValue2 - endValue;
        }*/
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::AddData(const DataPair& pair, Core::TableFormatter& table, int indent) const
{
    auto data = pair.second;
    auto indentStr = std::string(data.indent, '-');
    table.AddRow({
        indentStr.append(pair.first),
        std::to_string(data.sum / data.count),
        std::to_string(data.max),
        std::to_string(data.min)
        });
    for (auto& child : m_Values)
        if (child.second.parent == pair.first)
            AddData(child, table, indent + 1);
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::Log()
{
    Core::TableFormatter table(4, m_Values.size(), 1);
    table.AddRow({ TTag::Name, "Mean", "Max", "Min" });

    for (auto& pair : m_Values)
        if (pair.second.parent == nullptr)
            AddData(pair, table, 0);
    LogPerf(table);
}

template<typename TTag, typename TSample, typename TValue>
void Sampler<TTag, TSample, TValue>::End()
{
    if (freq++ >= m_LogFreqCount)
    {
        Log();
        m_Stack = std::stack<StackData>();
        m_Values.clear();
        freq = 0;
    }
}