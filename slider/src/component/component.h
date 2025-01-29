#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H

#include <memory>
#include <functional>

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