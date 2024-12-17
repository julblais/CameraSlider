#ifndef TIMER_H
#define TIMER_H

#include <functional>

namespace Utils
{
    class Timer
    {
        public:
            static void Update(unsigned int appTimeMs);

            Timer(const char* name);
            Timer(const char* name, std::function<void(unsigned long time)> callback, unsigned long delay);
            ~Timer();

            void Start();
            void Stop();
            unsigned int Delta() const;

        private:
            void ProcessCallback(unsigned long appTimeMs);
            
            const char* const m_Name;
            bool m_Started;
            unsigned long m_Delay;
            unsigned long m_AppTimeMs;
            std::function<void(unsigned long)> m_Callback;
    };

}
#endif