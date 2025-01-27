#include "app_base.h"
#include "app.h"

using namespace Slider;

template<>
std::unique_ptr<AppBase> Slider::AppCreator<AppConfig>::Create(const AppConfig &config)
{
    return std::unique_ptr<AppBase>(new App(config));
}
