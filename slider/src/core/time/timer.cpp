#include "timer.h"
#include "src/debug.h"
#include "src/core/utils/queue.h"
#include "esp_timer.h"

#ifdef ARDUINO_ARCH_ESP32 

using namespace Core;

const int TIMER_QUEUE_LENGTH = 20;
Queue<Timer::UserData*, TIMER_QUEUE_LENGTH> s_Queue { "Timer queue" };

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

#endif