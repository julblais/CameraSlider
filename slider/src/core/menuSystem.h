#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "debug.h"
#include "display.h"
#include "symbols.h"

#include <vector>

namespace Core
{
    class MenuCommand
    {
    public:
        enum class Button : char
        {
            Left,
            Right,
            Select
        };

        enum class DescriptionType : char
        {
            None,
            Options,
            Action
        };

        struct TitlePrefix : Printable
        {
            size_t printTo(Print& p) const override;
        };

        struct DescriptionPrefix : Printable
        {
            DescriptionPrefix(const DescriptionType type) : type(type) {}
            size_t printTo(Print& p) const override;

        private:
            DescriptionType type;
        };

        static constexpr auto ButtonLeft = Button::Left;
        static constexpr auto ButtonRight = Button::Right;
        static constexpr auto ButtonSelect = Button::Select;

/*
        template <typename... TArgs>
        static void PrintTitle(Display* display, TArgs&&... message);

        template <typename... TArgs>
        static void PrintDescription(Display* display, const DescriptionType type, TArgs&&... message);
*/
        virtual ~MenuCommand() = default;
        virtual void Print(Display* display) const = 0;
        virtual void Invoke(const Button command) = 0;

        virtual void OnOpenMenu() {}
        virtual void OnCloseMenu() {}
        virtual void OnShow() {}
        virtual void OnUpdate() {}
        virtual void OnHide() {}
    };

    class MenuSystem
    {
    public:
        MenuSystem();
        ~MenuSystem();

        void Open();
        void Close();
        void Update();
        void Print(Display* display) const;

        void AddCommand(MenuCommand* command);
        bool IsHidden() const { return !m_IsOpened; }
        bool IsShown() const { return m_IsOpened; }

        void Up();
        void Down();
        void Left();
        void Right();
        void Select();

    private:
        std::vector<MenuCommand*> m_Commands;
        int m_Index;
        bool m_IsOpened;
    }; /*

    template <typename... TArgs>
    void MenuCommand::PrintTitle(Display* display, TArgs&&... message)
    {
        const IO::SymbolHandle symbol = IO::SymbolHandle(IO::Symbol::UpDownArrows);
        const auto upDownArrows = display->PrintLine(" ", symbol, upDownArrows, message...);
    }

    template <typename... TArgs>
    void MenuCommand::PrintDescription(Display* display, const DescriptionType type, TArgs&&... message)
    {
        switch (type)
        {
        case DescriptionType::Options:
            {
                const auto leftRightArrows = display->GetSymbol(Symbol::LeftRightArrows);
                display->PrintLine(1, " ", leftRightArrows, message...);
                break;
            }
        case DescriptionType::Action:
            {
                const auto leftRightArrows = display->GetSymbol(Symbol::RightArrow);
                display->PrintLine(1, " ", leftRightArrows, message...);
                break;
            }
        default:
            {
                display->PrintLine(1, " ", message...);
                break;
            }
        }
    }*/
}

#endif
