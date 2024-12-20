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
            void SendEvent(std::function<void(TListener*)> event) const;
            void SendEventWithCapture(std::function<bool(TListener*)> event) const;
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
    inline void EventSource<TListener>::SendEvent(std::function<void(TListener *)> event) const
    {
        for (auto listener : m_Listeners)
            event(listener);
    }

    template <typename TListener>
    inline void EventSource<TListener>::SendEventWithCapture(std::function<bool(TListener *)> event) const
    {
        for (auto listener : m_Listeners)
            if (event(listener))
                return;
    }
}

#endif