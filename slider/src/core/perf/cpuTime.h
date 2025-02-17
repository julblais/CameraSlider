#ifndef CPUTIME_H
#define CPUTIME_H

#include "perf.h"

namespace Performance
{
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
}

#endif