#ifndef TIMER_H
#define TIMER_H

class Timer
{
    public:
        Timer();

        void Start();
        unsigned int GetTimeMs();
        void Stop();

    private:
        unsigned int m_AppTimeMs;
};

#endif