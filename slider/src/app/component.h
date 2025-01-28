#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <memory>
#include <functional>

namespace Slider
{
    class Component
    {
        public:
            virtual void Setup() = 0;
            virtual void Update() = 0;
    };

    class ComponentWrapper : public Component
    {
        public:
            virtual void Setup() override;
            virtual void Update() override;

            template <typename T, typename... TArgs>
            static std::unique_ptr<ComponentWrapper> Create(TArgs&&... args);

        private:
            using TSetup = std::function<void()>;
            using TUpdate = std::function<void()>;
            
            ComponentWrapper(const TSetup& setup, const TUpdate& update)
                : m_SetupMethod(setup), m_UpdateMethod(update) {}

        private:
            const TSetup m_SetupMethod;
            const TUpdate m_UpdateMethod;
    };

    
    template <typename T, typename... TArgs>
    std::unique_ptr<ComponentWrapper> ComponentWrapper::Create(TArgs&&... args)
    {
        auto ptr = new T(std::forward<TArgs>(args)...);
        auto base = std::unique_ptr<T>(ptr);
        auto setup = [base]() { base->Setup(); };
        auto update = [base]() { base->Update(); };
        auto component = new ComponentWrapper(setup, update);
        return std::unique_ptr<ComponentWrapper>(component);
    }
}

#endif