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

        static constexpr auto ButtonLeft = Button::Left;
        static constexpr auto ButtonRight = Button::Right;
        static constexpr auto ButtonSelect = Button::Select;

        static Display& TitlePrefix(Display& display);
        static Display& DescriptionPrefix(Display& display, const DescriptionType type);

        virtual ~MenuCommand() = default;
        virtual void Print(Display& display) const = 0;
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
    };
}

#endif
