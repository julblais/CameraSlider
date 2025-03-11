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

        Timer();
        ~Timer();
        Timer(const Timer& timer) = delete;
        Timer& operator=(const Timer& timer) = delete;
        Timer(Timer&&);
        Timer& operator=(Timer&&);

        static Timer Create(const char* name, Callback callback);
        static void FireAndForget(const char* name, Time delayMs, Callback callback);
        void Start(Time delayMs, bool periodic = false) const;
        void Stop() const;
        void Restart(Time delayMs) const;
        bool IsRunning() const;

    private:
        Timer(const Id id, const char* name, esp_timer_handle_t handle);
        friend class TimerComponent;

        const char* m_Name;
        esp_timer_handle_t m_Handle;
        Id m_Id;
    };
}

#endif
#endif