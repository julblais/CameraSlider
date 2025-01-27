#include "app_base.h"
#include "app.h"

using namespace Slider;

std::unique_ptr<AppBase> Slider::AppCreator::Create(const AppConfig &config)
{
    return std::unique_ptr<AppBase>(new App(config));
}
