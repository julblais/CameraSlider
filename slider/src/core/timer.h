#ifndef TIMER_H
#define TIMER_H

#ifdef ARDUINO_ARCH_ESP32

#include "time/time.h"
#include "component.h"
#include <memory>
#include "src/core/utils/function.h"

struct esp_timer;
typedef esp_timer* esp_timer_handle_t;

namespace Core
{
    class TimerComponent : public Component
    {
        void Setup() override;
        void Update() override;
    };

    class Timer
    {
    public:
        using Callback = function<void()>;
        using Id = intptr_t;

        Timer();
        ~Timer();
        Timer(const Timer& timer) = delete;
        Timer& operator=(const Timer& timer) = delete;
        Timer(Timer&&);
        Timer& operator=(Timer&&);

        static Timer Create(const char* name, Callback callback);
        static void FireAndForget(const char* name, const Time delayMs, Callback callback);
        void Start(const Time delayMs, bool periodic = false) const;
        void Stop() const;
        void Restart(const Time delayMs) const;
        bool IsRunning() const;

    private:
        Timer(const Id id, const char* name, const esp_timer_handle_t handle);
        friend class TimerComponent;

        const char* m_Name;
        esp_timer_handle_t m_Handle;
        Id m_Id;
    };
}

#endif
#endif
