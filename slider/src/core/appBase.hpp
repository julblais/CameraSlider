#ifndef APPBASE_HPP
#define APPBASE_HPP

#include "appBase.h"

template <class TComponent>
void Core::AppBase::AddComponent(TComponent* component)
{
    m_Components.push_back(component);
}

template <class TComponent, typename... TArgs>
TComponent* Core::AppBase::AddComponent(TArgs&&... args)
{
    auto ptr = new TComponent(std::forward<TArgs>(args)...);
    m_Components.push_back(ptr);
    return ptr;
}

#endif // APPBASE_HPP
