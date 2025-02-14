#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
#include <list>
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

    using Time = unsigned long;
    struct TimerObj;

    class TimerComponent : public Component
    {
        friend struct TimerObj;

    public:
        using TimerCallback = std::function<void(Time time)>;

        virtual void Setup() override;
        virtual void Update() override;

        TimerComponent();
        Time GetCurrentTime();
        TimerObj Add(const char* name, TimerCallback callback, Time delay);

    private:
        struct TimerData
        {
            TimerData(const TimerCallback cb, Time delay, Time startTime)
                : cb(cb), delay(delay), startTime(startTime)
            {}
            TimerCallback cb;
            Time delay;
            Time startTime;
        };
        using Iterator = std::list<TimerData>::iterator;
        using Container = std::list<TimerData>;
        Time m_TimeMs;
        Container m_Callbacks;
    };

    struct TimerObj
    {
        friend class TimerComponent;
    public:
        void Start();
        void Stop();
        void Remove();
    private:
        TimerObj(TimerComponent::Iterator itr, TimerComponent::Container* t);
        TimerComponent::Iterator m_Id;
        TimerComponent::Container* m_Timer;
    };
}
#endif