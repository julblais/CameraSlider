#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

TimerObj::TimerObj(TimerComponent::Iterator itr, TimerComponent::Container* t)
    : m_Timer(t), m_Id(itr)
{}

TimerObj::TimerObj()
    : m_Timer(nullptr), m_Id()
{}

TimerComponent::TimerComponent()
    :m_TimeMs(0), m_Callbacks()
{}

void TimerComponent::Setup()
{}

void TimerComponent::Update()
{
    m_TimeMs = millis();
    for (auto& cb : m_Callbacks)
    {
        if (!cb.cb) return;

        const auto delta = cb.startTime < m_TimeMs ? m_TimeMs - cb.startTime : 0u;
        if (delta >= cb.delay)
        {
            LogDebug("Timer \"", cb.name, "\" activated at: ", m_TimeMs);
            cb.cb(delta);
            cb.startTime = ULONG_MAX;
        }
    }
}

void TimerObj::Start()
{
    if (m_Timer != nullptr)
        m_Id->startTime = millis();
}

void TimerObj::Stop()
{
    if (m_Timer != nullptr)
        m_Id->startTime = ULONG_MAX;
}

void TimerObj::Remove()
{
    if (m_Timer != nullptr)
    {
        m_Timer->erase(m_Id);
        m_Timer = nullptr;
    }
}

TimerObj TimerComponent::Add(const char* name, TimerCallback callback, Time delay)
{
    auto t = m_Callbacks.emplace(m_Callbacks.cend(), name, callback, ULONG_MAX, delay);
    return TimerObj(t, &m_Callbacks);
}

Time TimerComponent::GetCurrentTime()
{
    return m_TimeMs;
}
