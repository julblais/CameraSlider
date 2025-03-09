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

        Timer();
        Timer(const Timer& timer) = delete;
        Timer& operator=(const Timer& timer) = delete;
        Timer(Timer&&) = default;
        Timer& operator=(Timer&&) = default;

        static Timer Create(const char* name, Callback callback);
        static void FireAndForget(const char* name, Time delayMs, Callback callback);
        void Start(Time delayMs, bool periodic = false) const;
        void Stop() const;
        void Restart(Time delayMs) const;
        bool IsRunning() const;

    private:
        Timer(UserData* userData);
        friend class TimerComponent;

        std::unique_ptr<UserData> m_UserData;
    };
}

#endif
#endif