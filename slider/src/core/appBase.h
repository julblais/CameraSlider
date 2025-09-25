#ifndef APP_BASE_H
#define APP_BASE_H

#include "component.h"
#include <vector>

namespace Core
{
    class AppBase
    {
    public:
        virtual ~AppBase();
        virtual void Setup() = 0;
        virtual void Update() = 0;

        template <class TComponent>
        void AddComponent(TComponent* component);
        template <class TComponent, typename... TArgs>
        TComponent* AddComponent(TArgs&&... args);

        void SetupComponents() const;
        void UpdateComponents() const;

    private:
        std::vector<Component*> m_Components;
    };

    template <class TConfig>
    struct AppCreator
    {
        static std::unique_ptr<AppBase> Create(const TConfig& config);
    };
}

#include "appBase.hpp"

#endif
