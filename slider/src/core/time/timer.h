#ifndef TIMER_H
#define TIMER_H

#include <functional>

namespace Utils
{
    class Timer
    {
    public:
        static void Update(unsigned long appTimeMs);

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

}
#endif