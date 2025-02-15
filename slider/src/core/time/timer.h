#ifndef TIMER_H
#define TIMER_H

#include "src/core/component/component.h"
#include <functional>
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

    private:
        struct TimerData
        {
            TimerData(const char* name, unsigned int id);
            const char* name;
            unsigned int id;
            TimerCallback cb;
            Time triggerTime;
        };
        void Add(const char* name, unsigned int id);
        void Start(unsigned int id, Time delay);
        void Stop(unsigned int id);
        void Remove(unsigned int id);
        void SetCallback(unsigned int id, const TimerComponent::TimerCallback& callback);
        
        std::vector<TimerData>::iterator Find(unsigned int id);
        Time m_TimeMs;
        std::vector<TimerData> m_Timers;
    };

    struct TimerObj
    {
        friend class TimerComponent;
    public:
        TimerObj(const char* name, TimerComponent* timer);
        ~TimerObj();
        void Start(Time delay);
        void Stop();
        void Remove();
        void SetCallback(const TimerComponent::TimerCallback& callback);
    private:
        const unsigned int m_Id;
        TimerComponent* const m_Timer;
    };
}
#endif