#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
#include <list>
#include <vector>

namespace Core
{
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
            TimerData(const char* name, const TimerCallback cb, Time startTime, Time delay)
                : name(name), cb(cb), delay(delay), startTime(startTime)
            {}
            const char* name;
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
        TimerObj();
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