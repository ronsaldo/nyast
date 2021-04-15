#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"
#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include "nyast/BaseClassLibrary/NativeClassRegistration.hpp"

namespace nyast
{

static std::once_flag defaultRuntimeEnvironmentInitialization;
static std::unique_ptr<RuntimeEnvironment> defaultRuntimeEnvironment;


RuntimeEnvironment::~RuntimeEnvironment()
{
}

class RuntimeEnvironmentImpl: public RuntimeEnvironment
{
public:
    RuntimeEnvironmentImpl();
    ~RuntimeEnvironmentImpl();

    static RuntimeEnvironment& getCurrent();

    void ensureInitialization();
    GarbageCollector& getGarbageCollector();

private:
    std::unique_ptr<GarbageCollector> garbageCollector;

    std::mutex environmentMutex;
    bool isInitialized = false;
};



RuntimeEnvironmentImpl::RuntimeEnvironmentImpl()
{
    garbageCollector.reset(GarbageCollector::createDefault());
}

RuntimeEnvironmentImpl::~RuntimeEnvironmentImpl()
{
}

void RuntimeEnvironmentImpl::ensureInitialization()
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
        defaultRuntimeEnvironment.reset(new RuntimeEnvironmentImpl());
    });

    return *defaultRuntimeEnvironment;
}

GarbageCollector & RuntimeEnvironmentImpl::getGarbageCollector()
{
    return *garbageCollector;
}

NYAST_CORE_EXPORT uint8_t* allocateAndInitializeObjectMemoryWith(size_t allocationSize, const std::function<void(uint8_t*)>& memoryInitializationFunction)
{
    return RuntimeEnvironment::getCurrent().getGarbageCollector().allocateAndInitializeObjectMemoryWith(allocationSize, memoryInitializationFunction);
}

} // End of namespace nyast
