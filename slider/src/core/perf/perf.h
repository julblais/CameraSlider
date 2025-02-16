#ifndef PERF_H
#define PERF_H

#include "esp_log.h"
#include "Print.h"
#include <utility>
#include <type_traits>

namespace Performance
{
    template <typename TTag, typename TSample, typename TValue>
    class Sampler : public Printable
    {
        using HasValue = decltype(std::declval<const TSample>().GetValue());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");
        using HasUnit = decltype(std::declval<const TSample>().GetUnit());
        static_assert(std::is_same<HasUnit, const char*>::value, "Incorrect return type from GetUnit");

    public:
        virtual ~Sampler() override = default;
        virtual size_t printTo(Print& p) const override;

        void AddSample(const TSample& sample);
        void Reset();
        TValue GetAverage();
        unsigned int GetSampleCount();

    private:
        unsigned int m_Count;
        TValue m_Sum;
        TValue m_Max;
        TValue m_Min;
    };

    struct CpuUsageTag
    {
        static constexpr const char* Name { "CpuTime" };
    };

    struct CpuUsage
    {
    public:
        CpuUsage();
        uint8_t GetValue() const;
        const char* GetUnit() const;
    private:

    };

    template class Sampler<CpuUsageTag, CpuUsage, uint8_t>;
    class CpuUsageSampler : public Sampler<CpuUsageTag, CpuUsage, uint8_t> {};

    struct CpuTimeTag
    {
        static constexpr const char* Name { "Cpu time" };
    };

    struct CpuTime
    {
    public:
        CpuTime();
        uint64_t GetValue() const;
        const char* GetUnit() const;
    private:
        uint64_t m_StartMicroseconds;
    };

    template class Sampler<CpuTimeTag, CpuTime, uint64_t>;
    class CpuTimeSampler : public Sampler<CpuTimeTag, CpuTime, uint64_t> {};


//adapted from: https://github.com/Carbon225/esp32-perfmon
}


#endif