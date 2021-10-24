#include "nyast/Oop.hpp"
#include "nyast/RuntimeEnvironment/RuntimeEnvironment.hpp"
#include "nyast/RuntimeEnvironment/GarbageCollector.hpp"
#include <thread>

namespace nyast
{

static thread_local StackRangeRecord *currentStackRangeRecord = nullptr;

StackRangeRecord *getCurrentStackRangeRecord()
{
    return currentStackRangeRecord;
}

void registerGlobalOopRoots(size_t rootCount, Oop *roots)
{
    RuntimeEnvironment::getCurrent().getGarbageCollector().registerGlobalOopRoots(rootCount, roots);
}

void unregisterGlobalOopRoots(size_t rootCount, Oop *roots)
{
    if(RuntimeEnvironment::hasCurrent())
        RuntimeEnvironment::getCurrent().getGarbageCollector().unregisterGlobalOopRoots(rootCount, roots);
}

void activateStackRangeRecord(StackRangeRecord *record)
{
    bool shouldAttemptInitialization = currentStackRangeRecord == nullptr;
    record->isActive = true;
    record->previous = currentStackRangeRecord;
    currentStackRangeRecord = record;

    auto& environment = RuntimeEnvironment::getCurrent();
    environment.getGarbageCollector().registerThisThread();
    if(shouldAttemptInitialization)
        environment.ensureInitialization();
}

void deactivateStackRangeRecord(StackRangeRecord *record)
{
    record->isActive = false;
    currentStackRangeRecord = record->previous;

    RuntimeEnvironment::getCurrent().getGarbageCollector().unregisterThisThread(currentStackRangeRecord);
}

void deactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(currentStackRangeRecord->isActive);
    currentStackRangeRecord->isActive = false;
    currentStackRangeRecord->endAddress = address;

    RuntimeEnvironment::getCurrent().getGarbageCollector().unregisterThisThread(currentStackRangeRecord);
}

void reactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(!currentStackRangeRecord->isActive);
    assert(currentStackRangeRecord->endAddress == address);
    RuntimeEnvironment::getCurrent().getGarbageCollector().registerThisThread();
    currentStackRangeRecord->isActive = true;
}

void gcSafePoint()
{
    assert(currentStackRangeRecord);
    RuntimeEnvironment::getCurrent().getGarbageCollector().safePoint();
}
} // End of namespace nyast
