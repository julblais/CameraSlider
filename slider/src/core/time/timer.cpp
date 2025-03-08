#include "timer.h"
#include "src/debug.h"
#include "src/core/utils/queue.h"
#include "esp_timer.h"

#ifdef ARDUINO_ARCH_ESP32 

using namespace Core;

const int TIMER_QUEUE_LENGTH = 20;
Queue<Timer::UserData*, TIMER_QUEUE_LENGTH> s_Queue { "Timer queue" };

struct Timer::UserData
{
public:
    UserData(const char* name, Callback callback, bool autoDelete);
    ~UserData();
    inline void SetHandle(esp_timer_handle_t handle) { m_Handle = handle; }
    inline esp_timer_handle_t GetHandle() { return m_Handle; }
    inline bool ShouldAutoDelete() { return m_AutoDelete; }
    void Invoke();
private:
    const char* m_Name;
    esp_timer_handle_t m_Handle;
    const Callback m_Callback;
    const bool m_AutoDelete;
};

void TimerComponent::Setup() {}

void TimerComponent::Update()
{
    Timer::UserData* userData;
    for (auto& userData : s_Queue)
    {
        userData->Invoke();
        if (userData->ShouldAutoDelete())
            delete userData;
    }
}

Timer::UserData::UserData(const char* name, Callback callback, bool autoDelete)
    : m_Name(name), m_Callback(std::move(callback)), m_Handle(nullptr), m_AutoDelete(autoDelete)
{}

Timer::UserData::~UserData()
{
    if (m_Handle != nullptr)
    {
        esp_timer_stop(m_Handle);
        auto result = esp_timer_delete(m_Handle);
        if (result != ESP_OK)
            LogError("Problem deleting timer ", m_Name, ", ", esp_err_to_name(result));
    }
}

void Timer::UserData::Invoke()
{
    LogDebug("Timer \"", m_Name, "\" activated at ", esp_timer_get_time() / 1000, "ms");
    m_Callback();
}

Timer::Timer()
    :m_UserData(nullptr)
{}

Timer::~Timer()
{
    if (m_UserData != nullptr)
        delete(m_UserData);
}

Timer::Timer(UserData* userData)
    : m_UserData(userData)
{}

void OnTimerTriggered(void* userData)
{
    s_Queue.push((Timer::UserData*)userData);
}

Timer::UserData* CreateTimer(const char* name, Timer::Callback cb, bool shouldDelete)
{
    auto userData = new Timer::UserData(name, std::move(cb), false);
    const esp_timer_create_args_t args = {
        .callback = &OnTimerTriggered,
        .arg = userData,
        .dispatch_method = esp_timer_dispatch_t::ESP_TIMER_TASK,
        .name = name
    };

    esp_timer* handle;
    auto result = esp_timer_create(&args, &handle);
    if (result != ESP_OK)
    {
        LogError("Cannot create timer: ", name, ", ", esp_err_to_name(result));
        return nullptr;
    }

    userData->SetHandle(handle);
    return userData;
}

Timer Timer::Create(const char* name, Callback cb)
{
    auto userData = CreateTimer(name, std::move(cb), false);
    return Timer(userData);
}

void Timer::FireAndForget(const char* name, Time delayMs, Callback cb)
{
    auto userData = CreateTimer(name, std::move(cb), true);
    esp_timer_start_once(userData->GetHandle(), delayMs * 1000);
}

void Timer::Start(Time delayMs, bool periodic) const
{
    if (m_UserData->GetHandle())
    {
        if (periodic)
            esp_timer_start_periodic(m_UserData->GetHandle(), delayMs * 1000);
        else
            esp_timer_start_once(m_UserData->GetHandle(), delayMs * 1000);
    }
}

void Timer::Stop() const
{
    if (m_UserData->GetHandle())
        esp_timer_stop(m_UserData->GetHandle());
}

void Timer::Restart(Time delay) const
{
    Stop();
    Start(delay);
}

bool Timer::IsRunning() const
{
    if (m_UserData->GetHandle())
        return esp_timer_is_active(m_UserData->GetHandle());
    return false;
}

#else

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

TimerComponent::TimerComponent()
    :m_TimeMs(0), m_Timers()
{
    m_Timers.reserve(10);
}

Timer TimerComponent::Create(const char* name, const Timer::Callback& callback)
{
    LogDebug("Creating timer: ", name);
    auto timer = Timer(name, this);
    m_Timers.emplace_back(name, timer.m_Id, callback);
    return timer;
}

void TimerComponent::Update()
{
    auto currentTime = GetTimeMs();
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

void TimerComponent::Start(Timer::Id id, Time delay, bool periodic)
{
    auto itr = Find(id);
    LogDebug("Starting timer: ", itr->name);
    itr->triggerTime = GetTimeMs() + delay;
}

void TimerComponent::Stop(Timer::Id id)
{
    auto itr = Find(id);
    LogDebug("Stopping timer: ", itr->name);
    itr->triggerTime = ULONG_MAX;
}

void TimerComponent::Remove(Timer::Id id)
{
    auto itr = Find(id);
    LogDebug("Removing timer: ", itr->name);
    m_Timers.erase(itr);
}

std::vector<TimerComponent::TimerData>::iterator TimerComponent::Find(Timer::Id timerId)
{
    auto itr = std::find_if(m_Timers.begin(), m_Timers.end(),
        [timerId](const TimerData& data) { return data.id == timerId; });
    assert(itr != m_Timers.end());
    return itr;
}

Time TimerComponent::GetCurrentTime() const
{
    return m_TimeMs;
}

TimerComponent::TimerData::TimerData(const char* name, Timer::Id id, const Timer::Callback& callback)
    : name(name), id(id), cb(callback), triggerTime(ULONG_MAX)
{}

#endif