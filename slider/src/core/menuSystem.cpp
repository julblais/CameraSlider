#include "menuSystem.h"
#include "symbols.h"

using namespace Core;

Display& MenuCommand::TitlePrefix(Display& display)
{
    return display << Symbols::UpDownArrows;
}

Symbol GetSymbol(const MenuCommand::DescriptionType type)
{
    switch (type)
    {
    case MenuCommand::DescriptionType::Options:
        return Symbols::LeftRightArrows;
    case MenuCommand::DescriptionType::Action:
        return Symbols::RightArrow;
    default:
        return Symbol();
    }
}

Display& MenuCommand::DescriptionPrefix(Display& display, const DescriptionType type)
{
    return display << ' ' << GetSymbol(type);
}

MenuSystem::MenuSystem() :
    m_Index(0),
    m_IsOpened(false) {}

MenuSystem::~MenuSystem()
{
    for (const auto command : m_Commands)
        delete command;
}

void MenuSystem::Open()
{
    m_IsOpened = true;
    m_Index = 0;
    for (const auto command : m_Commands)
        command->OnOpenMenu();
    m_Commands[m_Index]->OnShow();
}

void MenuSystem::Close()
{
    m_Commands[m_Index]->OnHide();
    for (const auto command : m_Commands)
        command->OnCloseMenu();
    m_Index = -1;
    m_IsOpened = false;
}

void MenuSystem::Update()
{
    if (m_IsOpened)
        m_Commands[m_Index]->OnUpdate();
}

void MenuSystem::Print(Display* display) const
{
    if (m_IsOpened)
    {
        display->SetCursor(0, 0);
        m_Commands[m_Index]->Print(*display);
    }
}

void MenuSystem::AddCommand(MenuCommand* command)
{
    m_Commands.emplace_back(command);
}

void MenuSystem::Up()
{
    const auto oldIdx = m_Index;
    const auto newIdx = m_Index - 1;
    if (newIdx < 0) //wrap
        m_Index = m_Commands.size() - 1;
    else
        m_Index = newIdx;

    if (m_Index != oldIdx)
    {
        m_Commands[oldIdx]->OnHide();
        m_Commands[m_Index]->OnShow();
    }
}

void MenuSystem::Down()
{
    const auto oldIdx = m_Index;
    m_Commands[m_Index]->OnShow();
    const auto newIdx = m_Index + 1;
    if (newIdx >= m_Commands.size()) //wrap
        m_Index = 0;
    else
        m_Index = newIdx;
    m_Commands[m_Index]->OnShow();

    if (m_Index != oldIdx)
    {
        m_Commands[oldIdx]->OnHide();
        m_Commands[m_Index]->OnShow();
    }
}

void MenuSystem::Left()
{
    m_Commands[m_Index]->Invoke(MenuCommand::ButtonLeft);
}

void MenuSystem::Right()
{
    m_Commands[m_Index]->Invoke(MenuCommand::ButtonRight);
}

void MenuSystem::Select()
{
    m_Commands[m_Index]->Invoke(MenuCommand::ButtonSelect);
}
