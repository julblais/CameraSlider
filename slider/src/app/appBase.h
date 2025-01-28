#ifndef APP_BASE_H
#define APP_BASE_H

#include <memory>
#include <vector>
#include "component.h"

namespace Slider
{
    class AppBase
    {
        public:
            virtual void Setup() = 0;
            virtual void Update() = 0;

            template <class TComponent>
            void AddComponent(TComponent* component);
            template <class TComponent, typename... TArgs>
            TComponent* AddComponent(TArgs&&... args);

            void SetupComponents();
            void UpdateComponents();

        private:
            std::vector<std::unique_ptr<Component>> m_Components;
    };

    template<class TConfig>
    struct AppCreator
    {
        static std::unique_ptr<AppBase> Create(const TConfig& config);
    };
    
    template <class TComponent>
    void AppBase::AddComponent(TComponent* component)
    {
        m_Components.emplace_back(component);
    }
    
    template <class TComponent, typename... TArgs>
    TComponent* AppBase::AddComponent(TArgs&&... args)
    {
        auto ptr = new TComponent(std::forward<TArgs>(args)...);
        m_Components.emplace_back(ptr);
        return ptr;
    }
}

#endif