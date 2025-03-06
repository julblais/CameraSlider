#ifndef ANIMATEDPRINTABLE_H
#define ANIMATEDPRINTABLE_H

#include <Print.h>
#include <initializer_list>
#include "src/core/time/time.h"

namespace Core
{
    class AnimatedPrintable : public Printable
    {
        using Messages = std::initializer_list<const char*>;
    public:
        AnimatedPrintable(const char* name, unsigned int interval, Messages messages);
        AnimatedPrintable(const AnimatedPrintable& other) = delete;
        AnimatedPrintable& operator=(const AnimatedPrintable& other) = delete;
        AnimatedPrintable(AnimatedPrintable&& other);
        AnimatedPrintable& operator=(AnimatedPrintable&& other);
        
        virtual ~AnimatedPrintable() override = default;
        virtual size_t printTo(Print& p) const override;
    private:
        Messages m_Messages;
        const unsigned int m_Interval;
        mutable Time m_LastTimeMs;
        mutable unsigned int m_CurrentMessageIndex;
    };
}
#endif