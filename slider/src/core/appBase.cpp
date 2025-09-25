#include "appBase.h"

using namespace Core;

AppBase::~AppBase()
{
    for (const auto component : m_Components)
    {
        delete component;
    }
}

void AppBase::SetupComponents() const
{
    for (const auto& component : m_Components)
        component->Setup();
}

void AppBase::UpdateComponents() const
{
    for (const auto& component : m_Components)
        component->Update();
}
