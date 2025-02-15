#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

static unsigned int m_IdGenerator;

Timer::Timer()
    : m_Timer(nullptr), m_Id(0)
{}

Timer::Timer(const char* name, TimeManager* timer)
    : m_Timer(timer), m_Id(++m_IdGenerator)
{}

void Timer::Start(Time delay) const { m_Timer->Start(m_Id, delay); }
void Timer::Stop() const { m_Timer->Stop(m_Id); }
void Timer::Remove() const { m_Timer->Remove(m_Id); }

TimeManager::TimeManager()
    :m_TimeMs(0), m_Timers()
{
    m_Timers.reserve(10);
}

Timer TimeManager::Create(const char* name, const Timer::Callback& callback)
{
    LogDebug("Creating timer: ", name);
    auto timer = Timer(name, this);
    m_Timers.emplace_back(name, timer.m_Id);
    return timer;
}

void TimeManager::Update()
{
    auto currentTime = millis();
    m_TimeMs = currentTime;
    for (auto& timer : m_Timers)
    {
        if (currentTime >= timer.triggerTime)
        {
            LogInfo("Timer \"", timer.name, "\" activated at: ", currentTime);
            if (timer.cb)
                timer.cb(currentTime);
            timer.triggerTime = ULONG_MAX;
        }
    }
}

void TimeManager::Start(Timer::Id id, Time delay)
{
    auto itr = Find(id);
    LogDebug("Starting timer: ", itr->name);
    itr->triggerTime = millis() + delay;
}

void TimeManager::Stop(Timer::Id id)
{
    auto itr = Find(id);
    LogDebug("Stopping timer: ", itr->name);
    itr->triggerTime = ULONG_MAX;
}

void TimeManager::Remove(Timer::Id id)
{
    auto itr = Find(id);
    LogDebug("Removing timer: ", itr->name);
    m_Timers.erase(itr);
}

std::vector<TimeManager::TimerData>::iterator TimeManager::Find(Timer::Id timerId)
{
    auto itr = std::find_if(m_Timers.begin(), m_Timers.end(),
        [timerId](const TimerData& data) { return data.id == timerId; });
    assert(itr != m_Timers.end());
    return itr;
}

Time TimeManager::GetCurrentTime() const
{
    return m_TimeMs;
}

TimeManager::TimerData::TimerData(const char* name, Timer::Id id)
    : name(name), id(id), cb(), triggerTime(ULONG_MAX)
{}