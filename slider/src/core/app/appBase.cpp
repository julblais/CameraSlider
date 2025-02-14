#include "appBase.h"

using namespace Core;

void AppBase::SetupComponents()
{
    for (const auto& component : m_Components)
        component->Setup();
}

void AppBase::UpdateComponents()
{
    for (const auto& component : m_Components)
        component->Update();
}