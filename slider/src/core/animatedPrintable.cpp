#include "animatedPrintable.h"
#include <utility>

using namespace Core;

AnimatedPrintable::AnimatedPrintable(const unsigned int interval, const Messages messages) :
    m_Messages(messages),
    m_Interval(interval),
    m_CurrentMessageIndex(0) {}

AnimatedPrintable::AnimatedPrintable(AnimatedPrintable&& other) :
    m_Messages(std::move(other.m_Messages)),
    m_Interval(other.m_Interval),
    m_CurrentMessageIndex(other.m_CurrentMessageIndex) {}

AnimatedPrintable& AnimatedPrintable::operator=(AnimatedPrintable&& other)
{
    m_Messages = std::move(other.m_Messages);
    m_CurrentMessageIndex = other.m_CurrentMessageIndex;
    return *this;
}

size_t AnimatedPrintable::printTo(Print& p) const
{
    if (m_LastTimeMs == 0)
        m_LastTimeMs = GetTimeMs();

    const auto currentTime = GetTimeMs();
    if (currentTime - m_LastTimeMs > m_Interval)
    {
        m_CurrentMessageIndex = (m_CurrentMessageIndex + 1) % m_Messages.size();
        m_LastTimeMs = currentTime;
    }
    return p.print(*(m_Messages.begin() + m_CurrentMessageIndex));
}

AnimatedPrintable AnimatedPrintable::CreateProgressDots(const unsigned int interval)
{
    return AnimatedPrintable(interval, {"", ".", "..", "..."});
}
