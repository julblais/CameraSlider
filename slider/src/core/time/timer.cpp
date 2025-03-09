#include "timer.h"
#include "src/debug.h"
#include "src/core/utils/queue.h"
#include "esp_timer.h"
#include <algorithm>
#include <vector>

#ifdef ARDUINO_ARCH_ESP32 

using namespace Core;

struct Handle
{
public:
    Handle(const char* name, const Timer::Id id, Timer::Callback callback, bool autoRemove);
    void Invoke() const;
    Timer::Id GetId() const { return m_Id; }
    const char* GetName() const { return m_Name; }
    bool ShouldAutoRemove() { return m_AutoRemove; }
    bool operator==(Timer::Id other) const { return m_Id == other; }
private:
    const char* m_Name;
    Timer::Callback m_Callback;
    Timer::Id m_Id;
    bool m_AutoRemove;
};

Handle::Handle(const char* name, const Timer::Id id, Timer::Callback callback, bool autoRemove)
    :m_Name(name), m_Id(id), m_Callback(std::move(callback)), m_AutoRemove(autoRemove)
{}

void Handle::Invoke() const
{
    if (m_Callback)
    {
        LogDebug("Timer \"", m_Name, "\" activated at ", esp_timer_get_time() / 1000, "ms");
        m_Callback();
    }
}

////////////////////////////////////////////

const int TIMER_QUEUE_LENGTH = 20;
std::vector<Handle> s_Handles {};
Queue<Timer::Id, TIMER_QUEUE_LENGTH> s_Queue { "Timer queue" };

void RemoveHandle(const Timer::Id id)
{
    auto it = s_Handles.begin();
    while (it != s_Handles.end())
    {
        if (it->GetId() == id)
        {
            s_Handles.erase(it);
            return;
        }
    }
    LogError("Cannot remove callback, id not found: ", id);
}

void TimerComponent::Setup() {}

void TimerComponent::Update()
{
    for (auto& id : s_Queue)
    {
        auto handle = std::find(s_Handles.begin(), s_Handles.end(), id);
        if (handle != s_Handles.end())
        {
            handle->Invoke();
            if (handle->ShouldAutoRemove())
            {
                RemoveHandle(handle->GetId());
                LogDebug("Removing fire and forget: ", handle->GetName());
            }
        }
        else
            LogWarning("Skipping callback for deleted timer id: ", id);
    }
}

Timer::Timer()
    :m_Id(0), m_Handle(nullptr), m_Name("NULL")
{}

Timer::Timer(const Timer::Id id, const char* name, esp_timer_handle_t handle)
    :m_Name(name), m_Id(id), m_Handle(handle)
{}

Timer::~Timer()
{
    if (m_Handle != nullptr)
    {
        RemoveHandle(m_Id);
        esp_timer_stop(m_Handle);
        auto result = esp_timer_delete(m_Handle);
        if (result != ESP_OK)
            LogError("Problem deleting timer ", m_Name, ", ", esp_err_to_name(result));
        LogInfo("Removed timer: ", m_Name, ", id: ", m_Id);
    }
}

Timer::Timer(Timer&& other)
{
    m_Name = other.m_Name;
    other.m_Name = nullptr;
    m_Handle = other.m_Handle;
    other.m_Handle = nullptr;
    m_Id = other.m_Id;
    other.m_Id = 0;
}

Timer& Timer::operator=(Timer&& other)
{
    m_Name = other.m_Name;
    other.m_Name = nullptr;
    m_Handle = other.m_Handle;
    other.m_Handle = nullptr;
    m_Id = other.m_Id;
    other.m_Id = 0;
    return *this;
}

void OnTimerTriggered(void* data)
{
    auto userData = reinterpret_cast<unsigned int>(data);
    s_Queue.push(userData);
}

esp_timer_handle_t CreateHandle(const char* name, Timer::Callback cb, bool autoRemove, Timer::Id& o_Id)
{
    static Timer::Id s_IdGenerator = 1;
    const auto id = s_IdGenerator++;

    const esp_timer_create_args_t args = {
        .callback = &OnTimerTriggered,
        .arg = (void*)id,
        .dispatch_method = esp_timer_dispatch_t::ESP_TIMER_TASK,
        .name = name
    };

    esp_timer_handle_t handle;
    auto result = esp_timer_create(&args, &handle);
    if (result != ESP_OK)
    {
        LogError("Cannot create timer: ", name, ", ", esp_err_to_name(result));
        return 0;
    }

    s_Handles.emplace_back(name, id, std::move(cb), autoRemove);
    o_Id = id;
    return handle;
}

Timer Timer::Create(const char* name, Callback cb)
{
    Timer::Id id = 0;
    auto handle = CreateHandle(name, std::move(cb), false, id);
    return Timer(id, name, handle);
}

void Timer::FireAndForget(const char* name, Time delayMs, Callback cb)
{
    Timer::Id id = 0;
    auto handle = CreateHandle(name, std::move(cb), true, id);
    esp_timer_start_once(handle, delayMs * 1000);
}

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