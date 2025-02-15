#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
#include <vector>

namespace Core
{
    using Time = unsigned long;

    struct TimerObj;

    class TimerManager : public Component
    {
        friend struct TimerObj;

    public:
        using TimerCallback = std::function<void(Time time)>;

        virtual void Setup() override;
        virtual void Update() override;

        TimerManager();
        Time GetCurrentTime();

    private:
        struct TimerId
        {
            TimerId();
            bool operator==(const TimerId& other) const;
            unsigned int id;
        };

        struct TimerData
        {
            TimerData(const char* name, TimerId id);
            const char* name;
            TimerId id;
            TimerCallback cb;
            Time triggerTime;
        };
        void Add(const char* name, TimerId id);
        void Start(TimerId id, Time delay);
        void Stop(TimerId id);
        void Remove(TimerId id);
        void SetCallback(TimerId id, const TimerManager::TimerCallback& callback);
        
        std::vector<TimerData>::iterator Find(TimerId id);
        Time m_TimeMs;
        std::vector<TimerData> m_Timers;
    };

    struct TimerObj
    {
        friend class TimerManager;
    public:
        TimerObj(const char* name, TimerManager* timer);
        ~TimerObj();
        void Start(Time delay);
        void Stop();
        void Remove();
        void SetCallback(const TimerManager::TimerCallback& callback);
    private:
        const TimerManager::TimerId m_Id;
        TimerManager* const m_Timer;
    };
}
#endif