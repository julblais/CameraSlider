#include "core/appBase.h"
#include "appConfig.h"

#if IS_SIMULATOR
#include "simulatorApp.h"
#else
#include "sliderApp.h"
#endif

using namespace Core;
using namespace Slider;

template <>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig& config)
{
#if IS_SIMULATOR
    return std::unique_ptr<AppBase>(new SimulatorApp(config));
#else
    return std::unique_ptr<AppBase>(new SliderApp(config));
#endif
}
