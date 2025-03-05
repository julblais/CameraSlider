#ifndef TIMER_H
#define TIMER_H

#ifdef ARDUINO_ARCH_ESP32 

#include "time.h"
#include "esp_timer.h"
#include "src/core/component/component.h"
#include <functional>
#include <memory>

namespace Core
{
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

        static Timer Create(const char* name, Callback callback);
        static void FireAndForget(const char* name, Time delayMs, Callback callback);
        void Start(Time delayMs, bool periodic = false) const;
        void Stop() const;
        void Restart(Time delayMs) const;
        bool IsRunning() const;

    private:
        struct UserData
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

        Timer(UserData* userData);
        friend class TimerComponent;
        static UserData* CreateTimer(const char* name, Callback cb, bool shouldDelete);

        std::unique_ptr<UserData> m_UserData;
    };
}

#else

#include "src/core/component/component.h"
#include <functional>
#include <vector>

namespace Core
{
    using Time = unsigned long;

    class TimerComponent;

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

        Timer(const char* name, TimerComponent* timer);
        Timer::Id m_Id;
        TimerComponent* m_Timer;
        friend class TimerComponent;
    };

    class TimerComponent : public Component
    {
    public:
        TimerComponent();
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