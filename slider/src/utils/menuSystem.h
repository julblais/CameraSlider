#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "src/debug.h"

#include <vector>
#include <memory>

namespace Utils
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

            MenuCommand() {};
            virtual ~MenuCommand() {}
            virtual const char* GetTitle()=0;
            virtual const char* GetDesc()=0;
            virtual void Invoke(MenuCommandButton command)=0;
    };

    struct MenuOutput
    {
        const char* title;
        const char* desc;

        MenuOutput(const char* title, const char* desc):
            title(title),
            desc(desc){}
    };

    class MenuSystem
    {
        public:
            MenuSystem();

            void Reset();
            void AddCommand(MenuCommand* command);

            void Up();
            void Down();
            void Left();
            void Right();
            void Select();

            MenuOutput GetOutput() const;
            
        private:
            std::vector<std::unique_ptr<MenuCommand>> m_Items;
            int m_Index;
    };
}

#endif