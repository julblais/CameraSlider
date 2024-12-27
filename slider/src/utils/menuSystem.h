#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include "src/debug.h"

#include <vector>
#include <memory>

namespace Utils
{
    enum MenuCommandButton
    {
        LEFT,
        RIGHT
    };

    class MenuCommand
    {
        public:
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

            void MoveUp();
            void MoveDown();
            void MoveLeft();
            void MoveRight();

            MenuOutput GetOutput() const;
            
        private:
            std::vector<std::unique_ptr<MenuCommand>> m_Items;
            int m_Index;
    };
}

#endif