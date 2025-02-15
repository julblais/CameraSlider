#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace Core;

static unsigned int m_IdGenerator;

TimerObj::TimerObj(const char* name, TimerComponent* timer)
    : m_Timer(timer), m_Id(++m_IdGenerator)
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

void TimerObj::SetCallback(const TimerComponent::TimerCallback& callback)
{
    m_Timer->SetCallback(m_Id, callback);
}

TimerComponent::TimerComponent()
    :m_TimeMs(0), m_Timers()
{
    m_Timers.reserve(10);
}

void TimerComponent::Setup()
{}

void TimerComponent::Update()
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

void TimerComponent::Add(const char* name, unsigned int id)
{
    LogDebug("Adding timer: ", name);
    m_Timers.emplace_back(name, id);
}

void TimerComponent::Start(unsigned int id, Time delay)
{
    auto itr = Find(id);
    LogDebug("Starting timer: ", itr->name);
    itr->triggerTime = millis() + delay;
}

void TimerComponent::Stop(unsigned int id)
{
    auto itr = Find(id);
    LogDebug("Stopping timer: ", itr->name);
    itr->triggerTime = ULONG_MAX;
}

void TimerComponent::Remove(unsigned int id)
{
    auto itr = Find(id);
    LogDebug("Removing timer: ", itr->name);
    m_Timers.erase(itr);
}

void TimerComponent::SetCallback(unsigned int id, const TimerComponent::TimerCallback& callback)
{
    auto itr = Find(id);
    itr->cb = callback;
}

std::vector<TimerComponent::TimerData>::iterator TimerComponent::Find(unsigned int id)
{
    auto itr = std::find_if(m_Timers.begin(), m_Timers.end(),
        [id](const TimerData& data) { return data.id == id; });
    assert(itr != m_Timers.end());
    return itr;
}

Time TimerComponent::GetCurrentTime()
{
    return m_TimeMs;
}

TimerComponent::TimerData::TimerData(const char* name, unsigned int id)
    : name(name), id(id), cb(), triggerTime(ULONG_MAX)
{}

