#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

static unsigned int m_IdGenerator;

Timer::Id::Id()
    : id(++m_IdGenerator)
{}

bool Timer::Id::operator==(const Timer::Id& other) const
{
    return other.id == id;
}

Timer::Timer(const char* name, TimeManager* timer)
    : m_Timer(timer), m_Id()
{
    m_Timer->Add(name, m_Id);
}

Timer::~Timer()
{
    m_Timer->Remove(m_Id);
}

void Timer::Start(Time delay) { m_Timer->Start(m_Id, delay); }
void Timer::Stop() { m_Timer->Stop(m_Id); }
void Timer::Remove() { m_Timer->Remove(m_Id); }

void Timer::SetCallback(const Timer::Callback& callback)
{
    m_Timer->SetCallback(m_Id, callback);
}

TimeManager::TimeManager()
    :m_TimeMs(0), m_Timers()
{
    m_Timers.reserve(10);
}

TimeManager::TimerData::TimerData(const char* name, Timer::Id id)
    : name(name), id(id), cb(), triggerTime(ULONG_MAX)
{}

void TimeManager::Update()
{
    auto currentTime = millis();
    m_TimeMs = currentTime;
    for (auto& timer : m_Timers)
    {
        if (currentTime >= timer.triggerTime)
        {
            LogInfo("Timer \"", timer.name, "\" activated at: ", m_TimeMs);
            if (timer.cb)
                timer.cb(currentTime);
            timer.triggerTime = ULONG_MAX;
        }
    }
}

void TimeManager::Add(const char* name, Timer::Id id)
{
    LogDebug("Adding timer: ", name);
    m_Timers.emplace_back(name, id);
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

void TimeManager::SetCallback(Timer::Id id, const Timer::Callback& callback)
{
    auto itr = Find(id);
    itr->cb = callback;
}

std::vector<TimeManager::TimerData>::iterator TimeManager::Find(Timer::Id timerId)
{
    auto itr = std::find_if(m_Timers.begin(), m_Timers.end(),
        [timerId](const TimerData& data) { return data.id == timerId; });
    assert(itr != m_Timers.end());
    return itr;
}

Time TimeManager::GetCurrentTime()
{
    return m_TimeMs;
}