#ifndef TIMER_H
#define TIMER_H

#ifdef ARDUINO_ARCH_ESP32 

#include "time.h"
#include "src/core/component/component.h"
#include <functional>
#include <memory>

struct esp_timer;
typedef struct esp_timer* esp_timer_handle_t;

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
        using Id = intptr_t;
        
        struct TimerTrace
        {
        public:
            TimerTrace(const char* name, const Id id, Callback callback);
            void Invoke();
            Id GetId() const { return m_Id; }
            const char* GetName() const { return m_Name; }
            bool operator==(Id other) const { return m_Id == other; }
        private:
            const char* m_Name;
            Callback m_Callback;
            Id m_Id;
        };

        Timer();
        ~Timer();
        Timer(const Timer& timer) = delete;
        Timer& operator=(const Timer& timer) = delete;
        Timer(Timer&&) = default;
        Timer& operator=(Timer&&) = default;

        static Timer Create(const char* name, Callback callback);
        //static void FireAndForget(const char* name, Time delayMs, Callback callback);
        void Start(Time delayMs, bool periodic = false) const;
        void Stop() const;
        void Restart(Time delayMs) const;
        bool IsRunning() const;

    private:
        Timer(const Id id, const char* name, esp_timer_handle_t handle);
        static Timer CreateTimer(const char* name, Timer::Callback cb, bool shouldDelete);
        friend class TimerComponent;

        const char* m_Name;
        esp_timer_handle_t m_Handle;
        Id m_Id;
    };
}

#endif
#endif