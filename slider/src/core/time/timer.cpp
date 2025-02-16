#include "timer.h"
#include "debug.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#ifdef ESP_32

using namespace Core;

const int QUEUE_LENGTH = 10;
QueueHandle_t s_Queue;

void TimerComponent::Setup()
{
    s_Queue = xQueueCreate(QUEUE_LENGTH, sizeof(Timer::UserData));
}

void TimerComponent::Update()
{
    Timer::UserData* userData;
    if (xQueueReceive(s_Queue, &userData, 0))
        userData->callback();
}

Timer::UserData::UserData(const char* name, const Callback& callback)
    : name(name), callback(callback)
{}

void Timer::timer_callback(void* userData)
{
    xQueueSend(s_Queue, &userData, 0);
}

Timer::Timer()
    :m_Handle(nullptr), m_UserData(nullptr)
{}

Timer::Timer(Timer&& timer)
{
    m_Handle = timer.m_Handle;
    m_UserData = std::move(timer.m_UserData);
    timer.m_Handle = nullptr;
    timer.m_UserData = nullptr;
}

Timer& Timer::operator=(Timer&& timer)
{
    m_Handle = timer.m_Handle;
    m_UserData = std::move(timer.m_UserData);
    timer.m_Handle = nullptr;
    timer.m_UserData = nullptr;
    return *this;
}

Timer::~Timer()
{
    if (m_Handle)
        esp_timer_delete(m_Handle);
}

Timer Timer::Create(const char* name, const Callback& callback)
{
    Timer timer;
    timer.m_UserData = std::unique_ptr<UserData>(new UserData(name, callback));

    esp_timer_create_args_t args;
    args.name = name;
    args.callback = timer_callback;
    args.arg = timer.m_UserData.get();
    args.dispatch_method = esp_timer_dispatch_t::ESP_TIMER_TASK;
    esp_timer* handle;

    auto result = esp_timer_create(&args, &handle);
    if (result != ESP_OK)
    {
        LogError("Cannot create timer: ", name, ", ", esp_err_to_name(result));
        return Timer();
    }
    timer.m_Handle = handle;
    return timer;
}

void Timer::Start(Time delayMs)
{
    esp_timer_start_once(m_Handle, delayMs * 1000);
}

void Timer::Stop()
{
    esp_timer_stop(m_Handle);
}

void Timer::Restart(Time delay)
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