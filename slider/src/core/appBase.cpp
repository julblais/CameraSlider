#include "appBase.h"

using namespace Core;

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
