#include "src/core/app/appBase.h"
#include "appConfig.h"

#if defined(IS_BRAIN)
#include "src/app/brain/brainApp.h"
#elif defined(IS_CONTROLLER)
#include "src/app/controller/controllerApp.h"
#else // IS_SIMULATOR
#include "src/app/simulator/simulatorApp.h"
#endif

using namespace Core;
using namespace Slider;

template<>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig & config)
{
    #if defined(IS_BRAIN)
    #include "src/app/brain/brainApp.h"
    return std::unique_ptr<AppBase>(new BrainApp(config));
    #elif defined(IS_CONTROLLER)
    return std::unique_ptr<AppBase>(new ControllerApp(config));
    #else // IS_SIMULATOR
    #include "src/app/simulator/simulatorApp.h"
    return std::unique_ptr<AppBase>(new SimulatorApp(config));
    #endif
}
