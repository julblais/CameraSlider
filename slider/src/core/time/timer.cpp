#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

static unsigned int m_IdGenerator;

TimerManager::TimerId::TimerId()
    : id(++m_IdGenerator)
{}

bool TimerManager::TimerId::operator==(const TimerId& other) const
{
    return other.id == id;
}

TimerManager::TimerData::TimerData(const char* name, TimerManager::TimerId id)
    : name(name), id(id), cb(), triggerTime(ULONG_MAX)
{}

TimerObj::TimerObj(const char* name, TimerManager* timer)
    : m_Timer(timer), m_Id()
{
    m_Timer->Add(name, m_Id);
}

TimerObj::~TimerObj()
{
    m_Timer->Remove(m_Id);
}

void TimerObj::Start(Time delay)
{
    m_Timer->Start(m_Id, delay);
}

void TimerObj::Stop()
{
    m_Timer->Stop(m_Id);
}

void TimerObj::Remove()
{
    m_Timer->Remove(m_Id);
}

void TimerObj::SetCallback(const TimerManager::TimerCallback& callback)
{
    m_Timer->SetCallback(m_Id, callback);
}

TimerManager::TimerManager()
    :m_TimeMs(0), m_Timers()
{
    m_Timers.reserve(10);
}

void TimerManager::Setup()
{}

void TimerManager::Update()
{
    auto currentTime = millis();
    m_TimeMs = currentTime;
    for (auto& timer : m_Timers)
    {
        if (!timer.cb) return;
        if (currentTime >= timer.triggerTime)
        {
            LogInfo("Timer \"", timer.name, "\" activated at: ", m_TimeMs);
            timer.cb(currentTime);
            timer.triggerTime = ULONG_MAX;
        }
    }
}

void TimerManager::Add(const char* name, TimerId id)
{
    LogDebug("Adding timer: ", name);
    m_Timers.emplace_back(name, id);
}

void TimerManager::Start(TimerId id, Time delay)
{
    auto itr = Find(id);
    LogDebug("Starting timer: ", itr->name);
    itr->triggerTime = millis() + delay;
}

void TimerManager::Stop(TimerId id)
{
    auto itr = Find(id);
    LogDebug("Stopping timer: ", itr->name);
    itr->triggerTime = ULONG_MAX;
}

void TimerManager::Remove(TimerId id)
{
    auto itr = Find(id);
    LogDebug("Removing timer: ", itr->name);
    m_Timers.erase(itr);
}

void TimerManager::SetCallback(TimerId id, const TimerManager::TimerCallback& callback)
{
    auto itr = Find(id);
    itr->cb = callback;
}

std::vector<TimerManager::TimerData>::iterator TimerManager::Find(TimerId timerId)
{
    auto itr = std::find_if(m_Timers.begin(), m_Timers.end(),
        [timerId](const TimerData& data) { return data.id == timerId; });
    assert(itr != m_Timers.end());
    return itr;
}

Time TimerManager::GetCurrentTime()
{
    return m_TimeMs;
}