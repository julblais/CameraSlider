#include "src/core/app/appBase.h"
#include "appConfig.h"

#if defined(IS_SIMULATOR)
#include "src/app/simulator/simulatorApp.h"
#elif defined(IS_CONTROLLER)
#include "src/app/controller/controllerApp.h"
#else  //IS_BRAIN
#include "src/app/brain/brainApp.h"
#endif

using namespace Core;
using namespace Slider;

template<>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig& config)
{
    #if defined(IS_SIMULATOR)
    return std::unique_ptr<AppBase>(new SimulatorApp(config));
    #elif defined(IS_CONTROLLER)
    return std::unique_ptr<AppBase>(new ControllerApp(config));
    #else //IS_BRAIN
    return std::unique_ptr<AppBase>(new BrainApp(config));
    #endif
}
