#include "src/core/app/appBase.h"
#include "appConfig.h"

#ifdef IS_BRAIN
#include "src/app/brain/brainApp.h"
#endif
#ifdef IS_CONTROLLER
#include "src/app/controller/controllerApp.h"
#endif
#ifdef IS_SIMULATOR
#include "src/app/simulator/simulatorApp.h"
#endif

using namespace Core;
using namespace Slider;

template<>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig & config)
{
    #ifdef IS_BRAIN
    #include "src/app/brain/brainApp.h"
    return std::unique_ptr<AppBase>(new BrainApp(config));
    #endif
    #ifdef IS_CONTROLLER
    return std::unique_ptr<AppBase>(new ControllerApp(config));
    #endif
    #ifdef IS_SIMULATOR
    #include "src/app/simulator/simulatorApp.h"
    return std::unique_ptr<AppBase>(new SimulatorApp(config));
    #endif
}
