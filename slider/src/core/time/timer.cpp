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

void TimerComponent::Update()
{
    m_TimeMs = millis();
    for (auto cb : m_Callbacks) //be careful when iterating on the vector/list/forward list
        ProcessCallback(m_TimeMs, cb);
}

Core::TimerComponent::TimerComponent()
    :m_TimeMs(0)
{}

TimerObj Core::TimerComponent::Add(const char* name, TimerCallback callback, Time delay)
{
    m_Callbacks.push_back(TimerData { callback, delay, m_TimeMs });
    return TimerObj();
}

bool TimerComponent::ProcessCallback(const Time current, const TimerData& callback)
{
    if (callback.cb)
    {
        const auto delta = callback.startTime < current ? current - callback.startTime : 0u;
        if (delta >= callback.delay)
        {
            LogDebug("Timer \"", m_Name, "\" activated at: ", appTimeMs);
            callback.cb(delta);
            return true;
        }
    }
    return false;
}

