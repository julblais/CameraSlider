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
        using HasValue = decltype(std::declval<TSample>().GetValue());
        using HasUnit = decltype(std::declval<TSample>().GetUnit());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");
        static_assert(std::is_same<HasUnit, const char*>::value, "Incorrect return type from GetUnit");

    public:
        Sampler();
        virtual ~Sampler() override;
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
        uint8_t GetValue();
        const char* GetUnit();
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
        uint64_t GetValue();
        const char* GetUnit();
    private:
        uint64_t m_StartMicroseconds;
    };

    template class Sampler<CpuTimeTag, CpuTime, uint64_t>;
    class CpuTimeSampler : public Sampler<CpuTimeTag, CpuTime, uint64_t> {};











    class MeasureTime
    {
        struct TimeSample
        {
            friend class MeasureTime;
            ~TimeSample();
        private:
            TimeSample(uint64_t* value, unsigned int* count);
            uint64_t* m_Value;
            unsigned int* m_Count;
            uint64_t m_Start;
        };

    public:
        MeasureTime();

        TimeSample CreateSample();
        void AddSample(uint64_t value);
        void Reset();
        uint64_t GetMicrosecondsAverage();
        float GetMillisecondsAverage();
        unsigned int GetSampleCount();

    private:
        unsigned int m_Count;
        uint64_t m_Value;
    };
}


//adapted from: https://github.com/Carbon225/esp32-perfmon



#endif