#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <set>
#include <functional>

static auto Timers = std::set<Utils::Timer*>();

void Utils::Timer::Update(unsigned int appTimeMs)
{
    for(auto timer : Timers)
        timer->ProcessCallback(appTimeMs);
}

Utils::Timer::Timer(const char* name) :
    m_Name(name),
    m_AppTimeMs(0),
    m_Started(false),
    m_Callback()
{
    Timers.emplace(this);
}

Utils::Timer::Timer(
    const char* name, std::function<void(unsigned long time)> callback, unsigned long delay) :
    Timer(name)
{
    m_Callback = callback;
    m_Delay = delay;
}

Utils::Timer::~Timer() 
{
    Timers.erase(this);
}

void Utils::Timer::Start()
{
    m_Started = true;
    m_AppTimeMs = millis();
    Debug.Log("Timer \"", m_Name, "\" started at: ", m_AppTimeMs);
}

void Utils::Timer::Stop()
{
    if (m_Started)
    {
        Debug.Log("Timer \"", m_Name, "\" stopped");
        m_Started = false;
        m_AppTimeMs = 0;
    }
}

unsigned int Utils::Timer::Delta() const
{
    return m_Started ? millis() - m_AppTimeMs : 0;
}

void Utils::Timer::ProcessCallback(unsigned long appTimeMs)
{
    if (m_Started && m_Callback)
    {
        auto delta = appTimeMs - m_AppTimeMs;
        if (delta >= m_Delay)
        {
            Debug.Log("Timer \"", m_Name, "\" activated at: ", appTimeMs);
            m_Callback(delta);
            Stop();
        }
    }
}
