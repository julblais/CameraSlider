#include "timer.h"
#include "src/debug.h"
#include "src/core/utils/queue.h"
#include "esp_timer.h"
#include <algorithm>
#include <vector>

#ifdef ARDUINO_ARCH_ESP32 

using namespace Core;

const int TIMER_QUEUE_LENGTH = 20;

std::vector<Timer::TimerTrace> s_Timers {};
Queue<Timer::Id, TIMER_QUEUE_LENGTH> s_Queue { "Timer queue" };

void RemoveTrace(const Timer::Id id)
{
    std::remove_if(s_Timers.begin(), s_Timers.end(), [id](const Timer::TimerTrace& trace) {
        return trace == id; });
}

void TimerComponent::Setup() {}

void TimerComponent::Update()
{
    for (auto& id : s_Queue)
    {
        auto foundTimer = std::find(s_Timers.begin(), s_Timers.end(), id);
        if (foundTimer != s_Timers.end())
            foundTimer->Invoke();
        else
            LogWarning("Trying to invoke callback on deleted timer: ", foundTimer->GetName());
        //if (userData->ShouldAutoDelete())
          //  delete userData;
    }
}

Timer::TimerTrace::TimerTrace(const char* name, const Timer::Id id, Timer::Callback callback)
    :m_Name(name), m_Id(id), m_Callback(callback)
{}

void Timer::TimerTrace::Invoke()
{
    if (m_Callback)
    {
        LogDebug("Timer \"", m_Name, "\" activated at ", esp_timer_get_time() / 1000, "ms");
        m_Callback();
    }
}

Timer::Timer()
    :m_Id(0), m_Handle(nullptr), m_Name("")
{}

Timer::Timer(const Timer::Id id, const char* name, esp_timer_handle_t handle)
    :m_Name(name), m_Id(id), m_Handle(handle)
{}

Timer::~Timer()
{
    if (m_Handle != nullptr)
    {
        RemoveTrace(m_Id);
        esp_timer_stop(m_Handle);
        auto result = esp_timer_delete(m_Handle);
        if (result != ESP_OK)
            LogError("Problem deleting timer ", m_Name, ", ", esp_err_to_name(result));
    }
}

void OnTimerTriggered(void* data)
{
    auto userData = reinterpret_cast<unsigned int>(data);
    s_Queue.push(userData);
}

Timer Timer::CreateTimer(const char* name, Timer::Callback cb, bool shouldDelete)
{
    static Timer::Id s_IdGenerator = 1;
    const auto id = s_IdGenerator++;

    const esp_timer_create_args_t args = {
        .callback = &OnTimerTriggered,
        .arg = (void*)id,
        .dispatch_method = esp_timer_dispatch_t::ESP_TIMER_TASK,
        .name = name
    };

    esp_timer* handle;
    auto result = esp_timer_create(&args, &handle);
    if (result != ESP_OK)
    {
        LogError("Cannot create timer: ", name, ", ", esp_err_to_name(result));
        return Timer();
    }

    s_Timers.emplace_back(name, id, cb);
    return Timer(id, name, handle);
}

Timer Timer::Create(const char* name, Callback cb)
{
    return CreateTimer(name, std::move(cb), false);
}
/*
void Timer::FireAndForget(const char* name, Time delayMs, Callback cb)
{
    auto userData = CreateTimer(name, std::move(cb), true);
    esp_timer_start_once(userData->GetHandle(), delayMs * 1000);
}*/

void Timer::Start(Time delayMs, bool periodic) const
{
    if (m_Handle)
    {
        if (periodic)
            esp_timer_start_periodic(m_Handle, delayMs * 1000);
        else
            esp_timer_start_once(m_Handle, delayMs * 1000);
    }
}

void Timer::Stop() const
{
    if (m_Handle)
        esp_timer_stop(m_Handle);
}

void Timer::Restart(Time delay) const
{
    Stop();
    Start(delay);
}

bool Timer::IsRunning() const
{
    if (m_Handle)
        return esp_timer_is_active(m_Handle);
    return false;
}

#endif