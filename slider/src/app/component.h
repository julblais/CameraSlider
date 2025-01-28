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

    template <typename T>
    class ComponentWrapper : public Component
    {
        using Method = std::function<void()>;

        public:
            ComponentWrapper(T* component, Method setup, Method update) 
                : m_Component(component), m_Setup(setup), m_Update(update) {}
            virtual void Setup() override;
            virtual void Update() override;

        private:
            std::unique_ptr<T> m_Component;
            const Method m_Setup;
            const Method m_Update;
    };

    template <typename T>
    void ComponentWrapper<T>::Setup()
    {
        m_Setup();
    }

    template <typename T>
    void ComponentWrapper<T>::Update()
    {
        m_Update();
    }
}

#endif