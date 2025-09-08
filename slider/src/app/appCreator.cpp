#include "src/core/app/appBase.h"
#include "appConfig.h"

#include "brainApp.h"

using namespace Core;
using namespace Slider;

template <>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig& config)
{
    return std::unique_ptr<AppBase>(new BrainApp(config));
}
