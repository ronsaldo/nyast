#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"
#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"

namespace nyast
{

static std::once_flag defaultRuntimeEnvironmentInitialization;
static std::unique_ptr<RuntimeEnvironment> defaultRuntimeEnvironment;

RuntimeEnvironment::RuntimeEnvironment()
{
    garbageCollector.reset(new GarbageCollector());
}

RuntimeEnvironment::~RuntimeEnvironment()
{
}

void RuntimeEnvironment::ensureInitialization()
{
    std::unique_lock<std::mutex> l(environmentMutex);
    if(isInitialized)
        return;

    AbstractNativeClassRegistry::get()->initializeRuntimeEnvironmentClasses();
    isInitialized = true;
}

RuntimeEnvironment& RuntimeEnvironment::getCurrent()
{
    std::call_once(defaultRuntimeEnvironmentInitialization, [](){
        defaultRuntimeEnvironment.reset(new RuntimeEnvironment());
    });

    return *defaultRuntimeEnvironment;
}

GarbageCollector &RuntimeEnvironment::getGarbageCollector()
{
    return *garbageCollector;
}

} // End of namespace nyast
