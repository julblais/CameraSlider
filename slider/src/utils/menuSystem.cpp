#include "menuSystem.h"
#include "debug.h"

Utils::MenuSystem::MenuSystem():
    m_Items(),
    m_Index(-1),
    m_IsOpened(false)
{
}

void Utils::MenuSystem::AddCommand(Utils::MenuCommand* command)
{
    m_Items.emplace_back(command);
}

void Utils::MenuSystem::Open()
{
    m_IsOpened = true;
    m_Index = 0;
}

void Utils::MenuSystem::Close()
{
    m_IsOpened = false;
    m_Index = -1;
}

void Utils::MenuSystem::MoveUp()
{
    if (!m_IsOpened) return;
    auto newIdx = m_Index-1;
    if (newIdx < 0) //wrap
        m_Index = m_Items.size()-1;
    else
        m_Index = newIdx;
}

void Utils::MenuSystem::MoveDown()
{
    if (!m_IsOpened) return;
    auto newIdx = m_Index+1;
    if (newIdx >= m_Items.size()) //wrap
        m_Index = 0;
    else
        m_Index = newIdx;
}

void Utils::MenuSystem::MoveLeft()
{
    if (!m_IsOpened) return;
    m_Items[m_Index]->Invoke(Utils::MenuCommandButton::LEFT);
}

void Utils::MenuSystem::MoveRight()
{
    if (!m_IsOpened) return;
    m_Items[m_Index]->Invoke(Utils::MenuCommandButton::RIGHT);
}

Utils::MenuOutput Utils::MenuSystem::GetOutput() const
{
    if (!m_IsOpened) return Utils::MenuOutput("", "");

    auto command = m_Items[m_Index].get();
    return Utils::MenuOutput(command->GetTitle(), command->GetDesc());
}
