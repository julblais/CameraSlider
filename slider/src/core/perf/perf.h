#ifndef PERF_H
#define PERF_H

#include "esp_log.h"
#include "Print.h"
#include <utility>
#include <type_traits>   

namespace Performance
{
    struct TestSample
    {
        int GetValue();
        const char* GetUnit();
    };

    struct TextTag { const char *text; };

    template <const TextTag& tag, typename TSample, typename TValue>
    class Sampler
    {
        using HasValue = decltype(std::declval<TSample>().GetValue());
        using HasUnit = decltype(std::declval<TSample>().GetUnit());
        static_assert(std::is_same<HasValue, TValue>::value, "Incorrect return type from GetValue");
        static_assert(std::is_same<HasUnit, const char*>::value, "Incorrect return type from GetUnit");

    public:
        Sampler();

        void AddSample(const TSample& sample);
        void Reset();
        TValue GetAverage();
        unsigned int GetSampleCount();
        void Print(Print& printer);

    private:
        unsigned int m_Count;
        TValue m_Sum;
        TValue m_Max;
        TValue m_Min;
    };


    struct TimeSample2
    {
    public:
        TimeSample2();
        int GetValue();
        const char* GetUnit();
    private:
        uint64_t m_StartMicroseconds;
    };
 


   //////////////////////////////

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


    static constexpr TextTag TestTag = { "Test" };
    using TestTimeSampler = Sampler<TestTag, TestSample, int>;

    
    static constexpr TextTag TestTime = { "TestTime" };
    using TestTimeSample = Sampler<TestTime, TimeSample2, int>;
}


//adapted from: https://github.com/Carbon225/esp32-perfmon



#endif