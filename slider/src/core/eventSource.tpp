#ifndef EVENTSOURCE_TPP
#define EVENTSOURCE_TPP

template<typename... TArgs>
void Core::EventSource<TArgs...>::AddListener(TListener listener)
{
    m_Listeners.push_back(std::move(listener));
}

template<typename... TArgs>
template<typename C>
auto Core::EventSource<TArgs...>::AddListener(C listener) -> decltype(listener.OnInputEvent(std::declval<TArgs...>()), void())
{
    m_Listeners.push_back([listener](TArgs... args) { return listener.OnInputEvent(args...); });
}

template<typename... TArgs>
template<typename C>
auto Core::EventSource<TArgs...>::AddListener(
    C* listener) -> decltype(listener->OnInputEvent(std::declval<TArgs...>()), void())
{
    m_Listeners.push_back([listener](TArgs... args) { return listener->OnInputEvent(args...); });
}

template<typename... TArgs>
void Core::EventSource<TArgs...>::SendEvent(TArgs&&... args) const
{
    for (auto listener: m_Listeners)
        if (listener(std::forward<TArgs>(args)...))
            return;
}
#endif // EVENTSOURCE_TPP
