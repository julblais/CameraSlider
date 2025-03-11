#ifndef EVT_SRC_H
#define EVT_SRC_H

#include "src/core/utils/function.h"
#include <vector>
#include <algorithm>

namespace Core
{
    template <typename... TArgs>
    class EventSource
    {
        using TListener = function<bool(TArgs...)>;

    public:
        EventSource() = default;
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
}

#include "eventSource.tpp"

#endif