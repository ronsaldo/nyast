#ifndef NYAST_RUNTIME_ENVIRONMENT_HPP
#define NYAST_RUNTIME_ENVIRONMENT_HPP

#include <memory>
#include <mutex>
#include "nyast/DllExport.hpp"

namespace nyast
{
NYAST_CORE_EXPORT class GarbageCollector;

/**
 * I am a global nyast runtime environment. I contain most of the core global
 * data structures that requires specialized initialization machinery.
 */
NYAST_CORE_EXPORT class RuntimeEnvironment
{
public:
    RuntimeEnvironment();
    ~RuntimeEnvironment();

    static RuntimeEnvironment& getCurrent();

    void ensureInitialization();
    GarbageCollector &getGarbageCollector();

private:
    std::unique_ptr<GarbageCollector> garbageCollector;

    std::mutex environmentMutex;
    bool isInitialized = false;
};

} // End of namespace nyast
#endif //NYAST_RUNTIME_ENVIRONMENT_HPP
