#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "src/debug.h"

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

        virtual ~MenuCommand() = default;
        virtual void OnOpenMenu() {};
        virtual void OnCloseMenu() {};
        virtual void OnShow() {};
        virtual void OnUpdate() {};
        virtual void OnHide() {};
        virtual const char* GetTitle() = 0;
        virtual const char* GetDesc() = 0;
        virtual void Invoke(MenuCommandButton command) = 0;
    };

    struct MenuOutput
    {
        const char* title;
        const char* desc;

        MenuOutput(const char* title, const char* desc);
    };

    class MenuSystem
    {
    public:
        enum class State { Hidden = 0, Intro = 1, Shown = 2 };

        MenuSystem();

        void Open();
        void Close();
        void Update();

        void AddCommand(MenuCommand* command);
        inline State GetState() const { return m_State; }
        inline bool IsHidden() const { return m_State == State::Hidden; }
        inline bool IsShown() const { return m_State == State::Shown; }
        inline bool IsIntro() const { return m_State == State::Intro; }

        inline void SetState(State state) { m_State = state; }

        void Up();
        void Down();
        void Left();
        void Right();
        void Select();

        MenuOutput GetOutput() const;

    private:
        std::vector<std::unique_ptr<MenuCommand>> m_Items;
        int m_Index;
        State m_State;
    };
}

#endif