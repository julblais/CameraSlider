#ifndef ANIMATEDPRINTABLE_H
#define ANIMATEDPRINTABLE_H

#include <Print.h>
#include <initializer_list>
#include "src/core/time/timer.h"

namespace Core
{
    class AnimatedPrintable : public Printable
    {
        using Messages = std::initializer_list<const char*>;
    public:
        AnimatedPrintable(const char* name, Messages messages);
        AnimatedPrintable(const AnimatedPrintable& other) = delete;
        AnimatedPrintable& operator=(const AnimatedPrintable& other) = delete;
        AnimatedPrintable(AnimatedPrintable&& other);
        AnimatedPrintable& operator=(AnimatedPrintable&& other);

        void Start(const unsigned int interval) const;
        void Stop() const;
        
        virtual ~AnimatedPrintable() override = default;
        virtual size_t printTo(Print& p) const override;
    private:
        Messages m_Messages;
        Timer m_Timer;
        mutable unsigned int m_Interval;
        unsigned int m_CurrentMessageIndex;
    };
}
#endif