#ifndef EVT_SRC_H
#define EVT_SRC_H

#include <vector>
#include <functional>

namespace Core
{
    template<typename... TArgs>
    class EventSource
    {
        using TListener = std::function<bool(TArgs...)>;

    public:
        EventSource() = default;
        void AddListener(TListener listener);

        // AddListener for classes that have OnInputEvent method
        template<typename C>
        auto AddListener(C listener) -> decltype(listener.OnInputEvent(std::declval<TArgs...>()), void());
        template<typename C>
        auto AddListener(C* listener) -> decltype(listener->OnInputEvent(std::declval<TArgs...>()), void());

    protected:
        void SendEvent(TArgs&&... args) const;

    private:
        std::vector<TListener> m_Listeners;
    };
}

#include "eventSource.tpp"

#endif
