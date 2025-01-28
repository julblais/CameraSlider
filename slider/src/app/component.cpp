#include "component.h"

using namespace Slider;

void ComponentWrapper::Setup() 
{
    m_SetupMethod();
}

void ComponentWrapper::Update() 
{
    m_UpdateMethod();
}
