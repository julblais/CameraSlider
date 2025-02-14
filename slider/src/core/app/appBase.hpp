using namespace Core;

template <class TComponent>
void AppBase::AddComponent(TComponent* component)
{
    m_Components.emplace_back(component);
}

template <class TComponent, typename... TArgs>
TComponent* AppBase::AddComponent(TArgs&&... args)
{
    auto ptr = new TComponent(std::forward<TArgs>(args)...);
    m_Components.emplace_back(ptr);
    return ptr;
}