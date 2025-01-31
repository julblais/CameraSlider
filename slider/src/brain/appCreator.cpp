#include "src/app/appBase.h"
#include "src/app/appConfig.h"

#define TEST_NETWORK

#ifdef TEST_NETWORK
    #include "src/test/network/netApp.h"
 #else
    #include "app.h"
#endif

using namespace Slider;

template<>
std::unique_ptr<AppBase> AppCreator<AppConfig>::Create(const AppConfig &config)
{
#ifdef TEST_NETWORK
    return std::unique_ptr<AppBase>(new NetApp(config));
#else
    return std::unique_ptr<AppBase>(new App(config));
#endif
}
