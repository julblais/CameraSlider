#ifndef PERF_H
#define PERF_H

#include "esp_log.h"
#include "Print.h"
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

namespace Performance
{
    template <typename TTag, typename TSample, typename TValue>
    class Sampler : public Printable
    {
        using Unit = decltype(TSample::Unit);
        static_assert(std::is_same<Unit, const char* const>::value, "Incorrect type for Unit");
        static_assert(std::is_default_constructible<TSample>::value, "Type should have empty ctor");
        using HasStart = decltype(std::declval<TSample>().Setup());
        using HasValue = decltype(std::declval<const TSample>().GetValue());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");

    public:
        Sampler(const unsigned int logFrequency);
        virtual ~Sampler() override = default;
        virtual size_t printTo(Print& p) const override;

        void Start();
        void BeginSample(const char* tag);
        void EndSample();
        void End();
    private:
        struct Data
        {
            TValue sum;
            TValue max;
            TValue min;
            unsigned int count;
        };
        struct StackData
        {
            TValue value;
            const char* name;
        };
        
        std::stack<StackData> m_Stack;
        std::map<const char*, Data> m_Values;
        
        unsigned int freq;
        const unsigned int m_LogFreqCount;
        TSample m_Sample;
    };

    struct CpuTime
    {
    public:
        struct Tag { static constexpr const char* Name { "cpu_time" }; };
        static constexpr const char* Unit { "us" };

        void Setup();
        uint64_t GetValue() const;
    };

    template class Sampler<CpuTime::Tag, CpuTime, uint64_t>;
    using CpuTimeSampler = Sampler<CpuTime::Tag, CpuTime, uint64_t>;
    static CpuTimeSampler CpuSampler(200u);

    /*
    struct CpuUsage
    {
    public:
        struct Tag { static constexpr const char* Name { "Cpu usage" }; };
        static constexpr const char* Unit { "%" };
        void Start();
        uint8_t GetValue() const;
    private:

    };

    template class Sampler<CpuUsage::Tag, CpuUsage, uint8_t>;
    using CpuUsageSampler = Sampler<CpuUsage::Tag, CpuUsage, uint8_t>;
    */
}
#include "perf.tpp"


#else
#define LogPerf(...) ;
#define MEASURE(sampler, func) func
#define TAKE_SAMPLE(sampler, tag, func) func
#endif //LOG_PERFORMANCE

#endif //PERF_H