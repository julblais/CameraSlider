#include "animatedPrintable.h"

using namespace Core;

AnimatedPrintable::AnimatedPrintable(const char* name, Messages messages)
    : m_Messages(messages), m_CurrentMessageIndex(0), m_Timer()
{
    m_Timer = Timer::Create(name, [this]() {
        m_CurrentMessageIndex = (m_CurrentMessageIndex + 1) % m_Messages.size();
        m_Timer.Restart(m_Interval);
    });
}

AnimatedPrintable::AnimatedPrintable(AnimatedPrintable&& other)
    :m_Messages(std::move(other.m_Messages)),
    m_Interval(other.m_Interval),
    m_Timer(std::move(other.m_Timer)),
    m_CurrentMessageIndex(other.m_CurrentMessageIndex)
{}

AnimatedPrintable& Core::AnimatedPrintable::operator=(AnimatedPrintable&& other)
{
    m_Messages = std::move(other.m_Messages);
    m_Interval = other.m_Interval;
    m_Timer = std::move(other.m_Timer);
    m_CurrentMessageIndex = other.m_CurrentMessageIndex;
    return *this;
}

void Core::AnimatedPrintable::Start(const unsigned int interval) const
{
    m_Interval = interval;
    m_Timer.Restart(interval);
}

void Core::AnimatedPrintable::Stop() const
{
    m_Timer.Stop();
}

size_t Core::AnimatedPrintable::printTo(Print& p) const
{
    return p.print(*(m_Messages.begin() + m_CurrentMessageIndex));
}
