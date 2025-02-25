#include "menuSystem.h"
#include "src/debug.h"

using namespace Core;

MenuSystem::MenuSystem() :
    m_Items(),
    m_Index(0),
    m_IsOpened(false)
{}

void MenuSystem::Open()
{
    m_IsOpened = true;
    m_Index = 0;
    for (auto& item : m_Items)
        item->OnOpenMenu();
    m_Items[m_Index]->OnShow();
}

void MenuSystem::Close()
{
    m_Items[m_Index]->OnHide();
    for (auto& item : m_Items)
        item->OnCloseMenu();
    m_Index = -1;
    m_IsOpened = false;
}

void MenuSystem::Update()
{
    if (m_IsOpened)
        m_Items[m_Index]->OnUpdate();
}

void MenuSystem::Print(Output::Display* display)
{
    if (m_IsOpened)
        m_Items[m_Index]->Print(display);
}

void MenuSystem::AddCommand(MenuCommand* command)
{
    m_Items.emplace_back(command);
}

void MenuSystem::Up()
{
    auto oldIdx = m_Index;
    auto newIdx = m_Index - 1;
    if (newIdx < 0) //wrap
        m_Index = m_Items.size() - 1;
    else
        m_Index = newIdx;

    if (m_Index != oldIdx)
    {
        m_Items[oldIdx]->OnHide();
        m_Items[m_Index]->OnShow();
    }
}

void MenuSystem::Down()
{
    auto oldIdx = m_Index;
    m_Items[m_Index]->OnShow();
    auto newIdx = m_Index + 1;
    if (newIdx >= m_Items.size()) //wrap
        m_Index = 0;
    else
        m_Index = newIdx;
    m_Items[m_Index]->OnShow();

    if (m_Index != oldIdx)
    {
        m_Items[oldIdx]->OnHide();
        m_Items[m_Index]->OnShow();
    }
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

void MenuCommand::PrintTitle(Output::Display* display, const char* title)
{
    const auto upDownArrows = display->GetSymbol(Output::Symbol::UpDownArrows);
    display->PrintLine(0, upDownArrows, title);
}

void MenuCommand::PrintDescription(Output::Display* display, const char* description)
{
    const auto leftRightArrows = display->GetSymbol(Output::Symbol::LeftRightArrows);
    display->PrintLine(1, " ", leftRightArrows, description);
}
