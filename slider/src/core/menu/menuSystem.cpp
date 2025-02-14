#include "menuSystem.h"
#include "src/debug.h"

Utils::MenuSystem::MenuSystem() :
    m_Items(),
    m_Index(0)
{}

void Utils::MenuSystem::AddCommand(Utils::MenuCommand* command)
{
    m_Items.emplace_back(command);
}

void Utils::MenuSystem::Reset()
{
    m_Index = 0;
}

void Utils::MenuSystem::Up()
{
    auto newIdx = m_Index - 1;
    if (newIdx < 0) //wrap
        m_Index = m_Items.size() - 1;
    else
        m_Index = newIdx;
}

void Utils::MenuSystem::Down()
{
    auto newIdx = m_Index + 1;
    if (newIdx >= m_Items.size()) //wrap
        m_Index = 0;
    else
        m_Index = newIdx;
}

void Utils::MenuSystem::Left()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonLeft);
}

void Utils::MenuSystem::Right()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonRight);
}

void Utils::MenuSystem::Select()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonSelect);
}

Utils::MenuOutput Utils::MenuSystem::GetOutput() const
{
    auto command = m_Items[m_Index].get();
    return Utils::MenuOutput(command->GetTitle(), command->GetDesc());
}

Utils::MenuOutput::MenuOutput(const char* title, const char* desc) :
    title(title),
    desc(desc)
{}
