#include "timer.h"
#include "src/debug.h"

#include <esp32-hal-timer.h>
#include <vector>
#include <algorithm>
#include <functional>

static auto Timers = std::vector<Utils::Timer*>();

void Utils::Timer::Update(unsigned long appTimeMs)
{
    for(auto timer : Timers)
        timer->ProcessCallback(appTimeMs);
}

Utils::Timer::Timer(const char* name) :
    m_Name(name),
    m_Callback(),
    m_StartTimeMs(0)
{
    Timers.push_back(this);
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
    auto position = std::find(Timers.begin(), Timers.end(), this);
    if (position != Timers.end())
        Timers.erase(position);
}

void Utils::Timer::Start()
{
    m_StartTimeMs = millis();
    Debug.Log("Timer \"", m_Name, "\" started at: ", m_StartTimeMs);
}

void Utils::Timer::Stop()
{
    if (m_StartTimeMs != 0)
    {
        Debug.Log("Timer \"", m_Name, "\" stopped");
        m_StartTimeMs = 0;
    }
}

void Utils::Timer::Trigger()
{
    ProcessCallback(ULONG_MAX);
}

unsigned int Utils::Timer::Delta() const
{
    return m_StartTimeMs != 0 ? millis() - m_StartTimeMs : 0;
}

void Utils::Timer::ProcessCallback(unsigned long appTimeMs)
{
    if (m_StartTimeMs != 0 && m_Callback)
    {
        const auto delta = m_StartTimeMs < appTimeMs ? appTimeMs - m_StartTimeMs : 0u;
        if (delta >= m_Delay)
        {
            Debug.Log("Timer \"", m_Name, "\" activated at: ", appTimeMs);
            m_Callback(delta);
            Stop();
        }
    }
}
