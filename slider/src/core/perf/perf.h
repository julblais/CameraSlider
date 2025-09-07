#ifndef PERF_H
#define PERF_H

#ifdef LOG_PERFORMANCE

#include "esp_log.h"
#include "src/core/utils/templateUtils.h"
#include <utility>
#include <type_traits>
#include <stack>
#include <map>

#define LogPerf(...) Debug::Log(__VA_ARGS__)
#define INIT_SAMPLER(sampler) sampler.Setup()
#define MEASURE(tag, func, ...) Performance::Measure(tag, func, __VA_ARGS__);\
Performance::Finish(__VA_ARGS__);
#define TAKE_SAMPLE(tag, func, ...) Performance::Measure(tag, func, __VA_ARGS__);

namespace Core
{
    class TableFormatter;
}

namespace Performance
{
    template<typename T, typename TFunc>
    void Measure(const char* tag, TFunc func, T&& sampler)
    {
        sampler.BeginSample(tag);
        func();
        sampler.EndSample();
    }

    template<typename T, typename... TArgs, typename TFunc>
    void Measure(const char* tag, TFunc func, T&& first, TArgs&&... args)
    {
        first.BeginSample(tag);
        Measure(tag, func, args...);
        first.EndSample();
    }

    template<typename... TArgs>
    void Finish(TArgs&&... args)
    {
        Core::PassParamPack { (args.Finish(), 1)... };
    }

    template<typename TTag, typename TSample, typename TValue, bool IsAbsolute = false>
    class Sampler
    {
        static_assert(std::is_default_constructible<TSample>::value, "Type should have empty ctor");
        using HasStart = decltype(std::declval<TSample>().Setup());
        using HasValue = decltype(std::declval<const TSample>().GetValue());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");

    public:
        Sampler(const unsigned int logFrequency);

        void Setup();
        void BeginSample(const char* tag);
        void EndSample();
        void Finish();

    private:
        void Log();

        struct Data
        {
            TValue sum;
            TValue max;
            TValue min;
            TValue current;
            unsigned int count;
            const char* parent;
        };

        struct StackData
        {
            TValue value;
            const char* tag;
            TValue bias;
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
        struct Tag
        {
            static constexpr const char* Name { "cpu time(us)" };
        };

        void Setup();
        uint64_t GetValue() const;
    };

    struct MemoryFreeSize
    {
    public:
        struct Tag
        {
            static constexpr const char* Name { "free memory(kb)" };
        };

        void Setup();
        int64_t GetValue() const;
    };

    using CpuTimeSampler = Sampler<CpuTime::Tag, CpuTime, uint64_t, false>;
    using MemoryFreeSampler = Sampler<MemoryFreeSize::Tag, MemoryFreeSize, int64_t, true>;
}

extern Performance::CpuTimeSampler CpuSampler;
extern Performance::MemoryFreeSampler MemorySampler;

#include "perf.tpp"

#else
#define LogPerf(...) ;
#define INIT_SAMPLER(sampler) ;
#define MEASURE(tag, func, ...) func();
#define TAKE_SAMPLE(tag, func, ...) func();
#endif //LOG_PERFORMANCE

#endif //PERF_H
