#ifndef TIMER_H
#define TIMER_H

#ifdef ESP_32

#include "esp_timer.h"
#include "src/core/component/component.h"
#include <functional>
#include <memory>

namespace Core
{
    using Time = unsigned long;

    class TimerComponent : public Component
    {
        virtual void Setup() override;
        virtual void Update() override;
    };

    class Timer
    {
    public:
        using Callback = std::function<void(void)>;

        Timer();
        Timer(const Timer& timer) = delete;
        Timer& operator=(const Timer& timer) = delete;
        Timer(Timer&& timer);
        Timer& operator=(Timer&& timer);
        ~Timer();

        static Timer Create(const char* name, const Callback& callback);
        void Start(Time delayMs);
        void Stop();
        void Restart(Time delayMs);
    private:
        struct UserData
        {
            UserData(const char* name, const Callback& callback);
            const char* name;
            const Callback callback;
        };
        static void timer_callback(void* callback);
        friend class TimerComponent;

        esp_timer* m_Handle;
        std::unique_ptr<UserData> m_UserData;
    };
}

#endif

#ifdef USE_CUSTOM_TIMER

#include "src/core/component/component.h"
#include <functional>
#include <vector>

namespace Core
{
    using Time = unsigned long;

    class TimeManager;

    struct Timer
    {
    public:
        using Callback = std::function<void(Time time)>;

        Timer();
        void Start(Time delay) const;
        void Stop() const;
        void Remove() const;

    private:
        using Id = unsigned int;

        Timer(const char* name, TimeManager* timer);
        Timer::Id m_Id;
        TimeManager* m_Timer;
        friend class TimeManager;
    };

    class TimeManager : public Component
    {
    public:
        TimeManager();
        Timer Create(const char* name, const Timer::Callback& callback);
        virtual void Update() override;
        Time GetCurrentTime() const;

    private:
        struct TimerData
        {
            TimerData(const char* name, Timer::Id id, const Timer::Callback& callback);
            const char* name;
            Timer::Id id;
            Timer::Callback cb;
            Time triggerTime;
        };

        void Start(Timer::Id id, Time delay);
        void Stop(Timer::Id id);
        void Remove(Timer::Id id);
        std::vector<TimerData>::iterator Find(Timer::Id id);

        Time m_TimeMs;
        std::vector<TimerData> m_Timers;
        friend struct Timer;
    };

}
#endif

#endif