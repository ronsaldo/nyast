#ifndef NYAST_RUNTIME_ENVIRONMENT_HPP
#define NYAST_RUNTIME_ENVIRONMENT_HPP

#include <memory>
#include <mutex>
#include "nyast/DllExport.hpp"

namespace nyast
{
class NYAST_CORE_EXPORT GarbageCollector;

/**
 * I am a global nyast runtime environment. I contain most of the core global
 * data structures that requires specialized initialization machinery.
 */
class NYAST_CORE_EXPORT RuntimeEnvironment
{
public:
    virtual ~RuntimeEnvironment();

    static RuntimeEnvironment& getCurrent();

    virtual void ensureInitialization() = 0;
    virtual GarbageCollector &getGarbageCollector() = 0;
};

} // End of namespace nyast
#endif //NYAST_RUNTIME_ENVIRONMENT_HPP
