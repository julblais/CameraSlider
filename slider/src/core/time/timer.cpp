#include "timer.h"
#include "debug.h"

#ifdef ESP_32


using namespace Core;

void timer_callback(void* callback)
{
    auto timerCallback = reinterpret_cast<Timer::Callback*>(callback);
    (*timerCallback)();
}

Timer Timer::Create(const char* name, const Callback& callback)
{
    Timer timer;
    timer.m_Callback = std::unique_ptr<Callback>(new Callback(callback));

    esp_timer_create_args_t args;
    args.name = name;
    args.callback = timer_callback;
    args.arg = timer.m_Callback.get();

    auto result = esp_timer_create(&args, &timer.m_Handle);
    if (result != ESP_OK)
    {
        LogDebug("Cannot create timer: ", name, ", ", esp_err_to_name(result));
        return Timer();
    }
    return timer;
}

void Core::Timer::Start(Time delayMs)
{
    esp_timer_start_once(m_Handle, delayMs * 1000);
}

void Core::Timer::Stop()
{
    esp_timer_stop(m_Handle);
}

void Core::Timer::Restart(Time delay)
{
    Stop();
    esp_timer_start_once(m_Handle, delay);
}


#endif

#ifdef USE_CUSTOM_TIMER

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
    m_Timers.emplace_back(name, timer.m_Id, callback);
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

TimeManager::TimerData::TimerData(const char* name, Timer::Id id, const Timer::Callback& callback)
    : name(name), id(id), cb(callback), triggerTime(ULONG_MAX)
{}

#endif
