#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
#include <vector>

namespace Core
{
    class Timer
    {
    public:
        static void UpdateS(unsigned long appTimeMs);


        Timer(const char* name);
        Timer(const char* name, std::function<void(unsigned long time)> callback, unsigned long delay);
        ~Timer();

        void Start();
        void Stop();
        void Trigger();
        unsigned int Delta() const;

    private:
        void ProcessCallback(unsigned long appTimeMs);

        const char* const m_Name;
        unsigned long m_Delay;
        unsigned long m_StartTimeMs;
        std::function<void(unsigned long)> m_Callback;
    };

    struct TimerObj;


    class TimerComponent : public Component
    {
    public:
        using Time = unsigned long;
        using TimerCallback = std::function<void(Time time)>;

    private:

        struct TimerData
        {
            TimerCallback cb;
            Time delay;
            Time startTime;
        };
    public:

        virtual void Setup() override;
        virtual void Update() override;

        // 1- with delay, once
        TimerComponent();

        TimerObj Add(const char* name, TimerCallback callback, Time delay);
        Time GetCurrentTime();

    private:
        bool ProcessCallback(const Time time, const TimerData& callback);
        Time m_TimeMs;
        std::vector<TimerData> m_Callbacks;
    };

    struct TimerObj
    {
    public:
        void Remove();
    private:
        unsigned int id;
        TimerComponent* m_Timer;
    };

}
#endif