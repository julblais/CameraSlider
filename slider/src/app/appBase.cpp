#include "appBase.h"

void Slider::AppBase::SetupComponents()
{
    for (auto& component : m_Components)
        component->Setup();
}

void Slider::AppBase::UpdateComponents()
{
    for (auto& component : m_Components)
        component->Update();
}