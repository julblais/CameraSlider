#ifndef EVT_SRC_H
#define EVT_SRC_H

#include <vector>
#include <functional>
#include <algorithm>

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
            std::vector<TListener*> m_Listeners;
    };

    template <typename TListener>
    inline EventSource<TListener>::EventSource():
        m_Listeners()
    {}

    template <typename TListener>
    inline void EventSource<TListener>::AddListener(TListener *listener)
    {
        m_Listeners.push_back(listener);
    }

    template <typename TListener>
    inline void EventSource<TListener>::RemoveListener(TListener *listener)
    {
        auto position = std::find(m_Listeners.begin(), m_Listeners.end(), listener);
        if (position != m_Listeners.end())
            m_Listeners.erase(position);
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