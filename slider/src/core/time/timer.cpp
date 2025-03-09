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
    Handle(const char* name, const Timer::Id id, Timer::Callback callback);
    void Invoke();
    Timer::Id GetId() const { return m_Id; }
    bool operator==(Timer::Id other) const { return m_Id == other; }
private:
    const char* m_Name;
    Timer::Callback m_Callback;
    Timer::Id m_Id;
};

Handle::Handle(const char* name, const Timer::Id id, Timer::Callback callback)
    :m_Name(name), m_Id(id), m_Callback(callback)
{}

void Handle::Invoke()
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
    std::remove_if(s_Handles.begin(), s_Handles.end(), [id](const Handle& trace) {
        return trace.GetId() == id; });
}

void TimerComponent::Setup() {}

void TimerComponent::Update()
{
    for (auto& id : s_Queue)
    {
        auto handle = std::find(s_Handles.begin(), s_Handles.end(), id);
        if (handle != s_Handles.end())
            handle->Invoke();
        else
            LogWarning("Skipping callback for deleted timer id: ", id);
        //if (userData->ShouldAutoDelete())
          //  delete userData;
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
        LogInfo("Removed timer: " , m_Name, ", id: ", m_Id);
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

    s_Handles.emplace_back(name, id, cb);
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