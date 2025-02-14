#include "src/core/app/appBase.h"
#include "appConfig.h"

#ifdef TEST_NETWORK
#include "src/brain/netApp.h"
#else
#include "app.h"
#endif

using namespace Core;
using namespace Slider;

template<>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig& config)
{
    #ifdef TEST_NETWORK
    #ifdef IS_RECEIVER
    return std::unique_ptr<AppBase>(new BrainApp(config));
    #endif
    #ifdef IS_SENDER
    return std::unique_ptr<AppBase>(new ControllerApp(config));
    #endif
    #else
    return std::unique_ptr<AppBase>(new App(config));
    #endif
}
