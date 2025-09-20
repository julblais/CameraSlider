#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "debug.h"
#include "display.h"

#include <vector>
#include <memory>

namespace Core
{
    enum class MenuCommandButton : char
    {
        LEFT,
        RIGHT,
        SELECT
    };

    class MenuCommand
    {
    public:
        static constexpr auto ButtonLeft = MenuCommandButton::LEFT;
        static constexpr auto ButtonRight = MenuCommandButton::RIGHT;
        static constexpr auto ButtonSelect = MenuCommandButton::SELECT;

        static void PrintTitle(Display* display, const char* title, bool drawArrows = true);
        static void PrintDescription(Display* display, const char* description, bool drawArrows = true);

        virtual ~MenuCommand() = default;
        virtual void Print(Display* display) const = 0;
        virtual void Invoke(const MenuCommandButton command) = 0;

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
        std::vector<std::unique_ptr<MenuCommand> > m_Items;
        int m_Index;
        bool m_IsOpened;
    };
}

#endif
