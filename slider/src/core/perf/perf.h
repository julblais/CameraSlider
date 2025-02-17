#ifndef PERF_H
#define PERF_H

#ifdef DEBUG_PERF

#include "esp_log.h"
#include "Print.h"
#include <utility>
#include <type_traits>

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
        Sampler(const unsigned int skipCount, const unsigned int logFrequency);
        virtual ~Sampler() override = default;
        virtual size_t printTo(Print& p) const override;

        inline void Start() { m_Sample.Start(); }
        void End();
        void Reset();
        TValue GetAverage() const;
        unsigned int GetSampleCount();

    private:
        struct Data {
            TValue sum;
            TValue max;
            TValue min;
            TValue last;
            unsigned int freq;
            unsigned int skip;
            unsigned int count;
        };
        TSample m_Sample;
        Data m_Data;
        const unsigned int m_SkipCount;
        const unsigned int m_LogFreqCount;
    };

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


//adapted from: https://github.com/Carbon225/esp32-perfmon
}

#endif
#endif