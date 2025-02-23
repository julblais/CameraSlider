#ifndef PERF_H
#define PERF_H

#include "esp_log.h"
#include <utility>
#include <type_traits>
#include <stack>
#include <map>

#ifdef LOG_PERFORMANCE
#define LogPerf(...) Debug::Logger().Log(__VA_ARGS__)
#define MEASURE(sampler, func) sampler.Start(); \
func \
sampler.End();
#define TAKE_SAMPLE(sampler, tag, func) sampler.BeginSample(tag);\
func \
sampler.EndSample();

namespace Core{
    class TableFormatter;
}

namespace Performance
{
    template <typename TTag, typename TSample, typename TValue>
    class Sampler
    {
        static_assert(std::is_default_constructible<TSample>::value, "Type should have empty ctor");
        using HasStart = decltype(std::declval<TSample>().Setup());
        using HasValue = decltype(std::declval<const TSample>().GetValue());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");

    public:
        Sampler(const unsigned int logFrequency);

        void Start();
        void BeginSample(const char* tag);
        void EndSample();
        void End();
    private:
        void Log();
        struct Data
        {
            TValue sum;
            TValue max;
            TValue min;
            unsigned int indent;
            unsigned int count;
            const char* parent;
        };
        struct StackData
        {
            TValue value;
            const char* tag;
        };

        using DataMap = std::map<const char*, Data>;
        using DataPair = std::pair<const char*, Data>;

        void AddData(const DataPair& pair, Core::TableFormatter& table, int indent) const;

        std::stack<StackData> m_Stack;
        DataMap m_Values;

        unsigned int freq;
        const unsigned int m_LogFreqCount;
        TSample m_Sample;
    };

    struct CpuTime
    {
    public:
        struct Tag { static constexpr const char* Name { "cpu_time(us)" }; };

        void Setup();
        uint64_t GetValue() const;
    };

    template class Sampler<CpuTime::Tag, CpuTime, uint64_t>;
    using CpuTimeSampler = Sampler<CpuTime::Tag, CpuTime, uint64_t>;

/*
    struct CpuUsage
    {
    public:
        struct Tag { static constexpr const char* Name { "Cpu usage" }; };
        void Start();
        uint8_t GetValue() const;
    private:

    };

    template class Sampler<CpuUsage::Tag, CpuUsage, uint8_t>;
    using CpuUsageSampler = Sampler<CpuUsage::Tag, CpuUsage, uint8_t>;
    */
}

extern Performance::CpuTimeSampler CpuSampler;

#include "perf.tpp"

#else
#define LogPerf(...) ;
#define MEASURE(sampler, func) func
#define TAKE_SAMPLE(sampler, tag, func) func
#endif //LOG_PERFORMANCE

#endif //PERF_H