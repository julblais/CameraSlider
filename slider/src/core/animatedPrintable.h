#ifndef ANIMATEDPRINTABLE_H
#define ANIMATEDPRINTABLE_H

#include <Print.h>
#include <initializer_list>
#include "time/time.h"

namespace Core
{
    class AnimatedPrintable
    {
        using Messages = std::initializer_list<const char*>;

    public:
        AnimatedPrintable(const unsigned int interval, const Messages messages);
        AnimatedPrintable(const AnimatedPrintable& other) = delete;
        AnimatedPrintable& operator=(const AnimatedPrintable& other) = delete;
        AnimatedPrintable(AnimatedPrintable&& other) noexcept;
        AnimatedPrintable& operator=(AnimatedPrintable&& other) noexcept;

        size_t printTo(Print& p) const;

        static AnimatedPrintable CreateProgressDots(const unsigned int interval = 300);

    private:
        Messages m_Messages;
        unsigned int m_Interval;
        mutable Time m_LastTimeMs;
        mutable unsigned int m_CurrentMessageIndex;
    };
}
#endif
