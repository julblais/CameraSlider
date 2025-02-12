#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <memory>
#include <functional>

//todo change namespace to make it more generic (like App, Engine, etc.)
namespace Slider
{
    class Component
    {
        public:
            virtual void Setup() {};
            virtual void Update() {};
    };
}

#endif