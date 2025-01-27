#ifndef APP_BASE_H
#define APP_BASE_H

#include <memory>

namespace Slider
{
    class AppBase
    {
        public:
            virtual void Setup() = 0;
            virtual void Update() = 0;
    };

    template<class TConfig>
    class AppCreator
    {
        public:
            static std::unique_ptr<AppBase> Create(const TConfig& config);
    };
}

#endif