#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

static auto Timers = std::vector<Timer*>();

void Timer::UpdateS(unsigned long appTimeMs)
{
    for (auto timer : Timers)
        timer->ProcessCallback(appTimeMs);
}

Timer::Timer(const char* name) :
    m_Name(name),
    m_Delay(0),
    m_Callback(),
    m_StartTimeMs(0)
{
    Timers.push_back(this);
}

Timer::Timer(
    const char* name, std::function<void(unsigned long time)> callback, unsigned long delay) :
    Timer(name)
{
    m_Callback = callback;
    m_Delay = delay;
}

Timer::~Timer()
{
    auto position = std::find(Timers.begin(), Timers.end(), this);
    if (position != Timers.end())
        Timers.erase(position);
}

void Timer::Start()
{
    m_StartTimeMs = millis();
    LogDebug("Timer \"", m_Name, "\" started at: ", m_StartTimeMs);
}

void Timer::Stop()
{
    if (m_StartTimeMs != 0)
    {
        LogDebug("Timer \"", m_Name, "\" stopped");
        m_StartTimeMs = 0;
    }
}

void Timer::Trigger()
{
    ProcessCallback(ULONG_MAX);
}

unsigned int Timer::Delta() const
{
    return m_StartTimeMs != 0 ? millis() - m_StartTimeMs : 0;
}

void Timer::ProcessCallback(unsigned long appTimeMs)
{
    if (m_StartTimeMs != 0 && m_Callback)
    {
        const auto delta = m_StartTimeMs < appTimeMs ? appTimeMs - m_StartTimeMs : 0u;
        if (delta >= m_Delay)
        {
            LogDebug("Timer \"", m_Name, "\" activated at: ", appTimeMs);
            m_Callback(delta);
            Stop();
        }
    }
}

void Core::TimerComponent::Setup()
{}

void TimerComponent::Update()
{
    m_TimeMs = millis();
    for (auto cb : m_Callbacks)
    {
        if (!cb.cb) return;

        const auto delta = cb.startTime < m_TimeMs ? m_TimeMs - cb.startTime : 0u;
        if (delta >= cb.delay)
        {
            LogDebug("Timer \"", m_Name, "\" activated at: ", appTimeMs);
            cb.cb(delta);
        }
    }
}

Core::TimerComponent::TimerComponent()
    :m_TimeMs(0), m_Callbacks()
{}

TimerObj::TimerObj(TimerComponent::Iterator itr, TimerComponent::Container* t)
    : m_Timer(t), m_Id(itr)
{}

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
    auto t = m_Callbacks.emplace(m_Callbacks.cend(), callback, delay, m_TimeMs);
    return TimerObj(t, &m_Callbacks);
}

Time TimerComponent::GetCurrentTime()
{
    return m_TimeMs;
}
