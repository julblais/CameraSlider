#include "menuSystem.h"
#include "src/debug.h"

using namespace Core;

MenuSystem::MenuSystem() :
    m_Items(),
    m_Index(0)
{}

void MenuSystem::AddCommand(MenuCommand* command)
{
    m_Items.emplace_back(command);
}

void MenuSystem::Reset()
{
    m_Index = 0;
}

void MenuSystem::Up()
{
    auto newIdx = m_Index - 1;
    if (newIdx < 0) //wrap
        m_Index = m_Items.size() - 1;
    else
        m_Index = newIdx;
}

void MenuSystem::Down()
{
    auto newIdx = m_Index + 1;
    if (newIdx >= m_Items.size()) //wrap
        m_Index = 0;
    else
        m_Index = newIdx;
}

void MenuSystem::Left()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonLeft);
}

void MenuSystem::Right()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonRight);
}

void MenuSystem::Select()
{
    m_Items[m_Index]->Invoke(MenuCommand::ButtonSelect);
}

MenuOutput MenuSystem::GetOutput() const
{
    auto command = m_Items[m_Index].get();
    return MenuOutput(command->GetTitle(), command->GetDesc());
}

MenuOutput::MenuOutput(const char* title, const char* desc) :
    title(title),
    desc(desc)
{}
