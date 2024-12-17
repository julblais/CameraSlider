#ifndef EVT_SRC_H
#define EVT_SRC_H

#include <set>
#include <functional>

namespace Utils
{
    template <typename TListener>
    class EventSource
    {
        public:
            EventSource();
            inline void AddListener(TListener* listener);
            inline void RemoveListener(TListener* listener);
        protected:
            void InvokeListeners(std::function<void(TListener*)> func) const;
        private:
            std::set<TListener*> m_Listeners;
    };

    template <typename TListener>
    inline EventSource<TListener>::EventSource():
        m_Listeners()
    {}

    template <typename TListener>
    inline void EventSource<TListener>::AddListener(TListener *listener)
    {
        m_Listeners.emplace(listener);
    }

    template <typename TListener>
    inline void EventSource<TListener>::RemoveListener(TListener *listener)
    {
        m_Listeners.erase(listener);
    }

    template <typename TListener>
    inline void EventSource<TListener>::InvokeListeners(std::function<void(TListener *)> func) const
    {
        for (auto listener : m_Listeners)
            func(listener);
    }
}

#endif