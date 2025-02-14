#include "appBase.h"

void Slider::AppBase::SetupComponents()
{
    for (const auto& component : m_Components)
        component->Setup();
}

void Slider::AppBase::UpdateComponents()
{
    for (const auto& component : m_Components)
        component->Update();
}