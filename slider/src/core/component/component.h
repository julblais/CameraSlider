#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <memory>

//todo change namespace to make it more generic (like App, Engine, etc.)
namespace Core
{
    class Component
    {
    public:
        virtual ~Component() = default;

        virtual void Setup() {};
        virtual void Update() {};
    };
}

#endif