#ifndef EVT_SRC_H
#define EVT_SRC_H

#include <vector>
#include <functional>
#include <algorithm>

namespace Core
{
    template <typename... TArgs>
    class EventSource
    {
        using TListener = std::function<bool(TArgs...)>;

    public:
        EventSource() : m_Listeners() {}
        void AddListener(TListener listener);

        // AddListener for classes that have OnInputEvent method
        template <typename C>
        auto AddListener(C listener) -> decltype(listener.OnInputEvent(std::declval<TArgs...>()), void());
        template <typename C>
        auto AddListener(C* listener) -> decltype(listener->OnInputEvent(std::declval<TArgs...>()), void());

    protected:
        void SendEvent(TArgs&&... args) const;

    private:
        std::vector<TListener> m_Listeners;
    };

    template <typename... TArgs>
    inline void EventSource<TArgs...>::AddListener(TListener listener)
    {
        m_Listeners.push_back(listener);
    }

    template <typename... TArgs>
    template <typename C>
    inline auto EventSource<TArgs...>::AddListener(C listener)
        -> decltype(listener.OnInputEvent(std::declval<TArgs...>()), void())
    {
        m_Listeners.push_back([listener](TArgs... args) { return listener.OnInputEvent(args...); });
    }

    template <typename... TArgs>
    template <typename C>
    inline auto EventSource<TArgs...>::AddListener(C* listener)
        -> decltype(listener->OnInputEvent(std::declval<TArgs...>()), void())
    {
        m_Listeners.push_back([listener](TArgs... args) { return listener->OnInputEvent(args...); });
    }

    template <typename... TArgs>
    inline void EventSource<TArgs...>::SendEvent(TArgs&&... args) const
    {
        for (auto listener : m_Listeners)
            if (listener(std::forward<TArgs>(args)...))
                return;
    }
}

#endif