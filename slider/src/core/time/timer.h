#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
#include <vector>

namespace Core
{
    using Time = unsigned long;

    class TimeManager;

    struct Timer
    {
        using Callback = std::function<void(Time time)>;

    public:
        Timer();
        void Start(Time delay);
        void Stop();
        void Remove();
        void SetCallback(const Callback& callback);

    private:
        struct Id
        {
            Id();
            Id(unsigned int id);
            bool operator==(const Id& other) const;
            unsigned int id;
        };

        Timer(const char* name, TimeManager* timer);
        Id m_Id;
        TimeManager* m_Timer;
        friend class TimeManager;
    };

    class TimeManager : public Component
    {
    public:
        TimeManager();
        Timer Create(const char* name, const Timer::Callback& callback);
        virtual void Update() override;
        Time GetCurrentTime();

    private:
        struct TimerData
        {
            TimerData(const char* name, Timer::Id id);
            const char* name;
            Timer::Id id;
            Timer::Callback cb;
            Time triggerTime;
        };

        void Start(Timer::Id id, Time delay);
        void Stop(Timer::Id id);
        void Remove(Timer::Id id);
        void SetCallback(Timer::Id id, const Timer::Callback& callback);

        std::vector<TimerData>::iterator Find(Timer::Id id);
        Time m_TimeMs;
        std::vector<TimerData> m_Timers;
        friend struct Timer;
    };

}
#endif