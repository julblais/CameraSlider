#ifndef PERF_H
#define PERF_H

#include "esp_log.h"
#include "Print.h"
#include <utility>
#include <type_traits>

#ifdef LOG_PERFORMANCE
#define LogPerf(...) Debug::Logger().Log(__VA_ARGS__)
#define SAMPLER_START(sampler) sampler.Start();
#define SAMPLER_END(sampler) sampler.End();
#define CREATE_SAMPLER(type, name, frequency) static Performance::type name(frequency);
#define MEASURE(sampler, func) sampler.Measure([]()func);

namespace Performance
{
    template <typename TTag, typename TSample, typename TValue>
    class Sampler : public Printable
    {
        using Unit = decltype(TSample::Unit);
        static_assert(std::is_same<Unit, const char* const>::value, "Incorrect type for Unit");
        static_assert(std::is_default_constructible<TSample>::value, "Type should have empty ctor");
        using HasStart = decltype(std::declval<TSample>().Start());
        using HasValue = decltype(std::declval<const TSample>().GetValue());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");

    public:
        Sampler(const unsigned int logFrequency);
        virtual ~Sampler() override = default;
        virtual size_t printTo(Print& p) const override;

        inline void Start() { m_Sample.Start(); }

        template <typename TFunc>
        inline void Measure(TFunc&& func)
        {
            Start();
            func();
            End();
        }

        void End();
        void Reset();
        TValue GetAverage() const;
        unsigned int GetSampleCount();

    private:
        struct Data
        {
            TValue sum;
            TValue max;
            TValue min;
            TValue last;
            unsigned int freq;
            unsigned int count;
        };
        TSample m_Sample;
        Data m_Data;
        const unsigned int m_LogFreqCount;
    };

    struct CpuTime
    {
    public:
        struct Tag { static constexpr const char* Name { "Cpu time" }; };
        static constexpr const char* Unit { "us" };

        void Start();
        uint64_t GetValue() const;
    private:
        uint64_t m_StartMicroseconds;
    };

    template class Sampler<CpuTime::Tag, CpuTime, uint64_t>;
    using CpuTimeSampler = Sampler<CpuTime::Tag, CpuTime, uint64_t>;

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
#define SAMPLER_START(sampler) ;
#define SAMPLER_END(sampler) ;
#define CREATE_SAMPLER(type, name, frequency) ;
#define MEASURE(sampler, func) func;
#endif //LOG_PERFORMANCE

#endif //PERF_H