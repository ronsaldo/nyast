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
    GarbageCollector::getCurrent().registerGlobalOopRoots(rootCount, roots);
}

void unregisterGlobalOopRoots(size_t rootCount, Oop *roots)
{
    GarbageCollector::getCurrent().unregisterGlobalOopRoots(rootCount, roots);
}

void activateStackRangeRecord(StackRangeRecord *record)
{
    bool shouldAttemptInitialization = currentStackRangeRecord == nullptr;
    record->isActive = true;
    record->previous = currentStackRangeRecord;
    currentStackRangeRecord = record;

    GarbageCollector::getCurrent().registerThisThread();
    if(shouldAttemptInitialization)
        RuntimeEnvironment::getCurrent().ensureInitialization();
}

void deactivateStackRangeRecord(StackRangeRecord *record)
{
    record->isActive = false;
    currentStackRangeRecord = record->previous;

    GarbageCollector::getCurrent().unregisterThisThread(currentStackRangeRecord);
}

void deactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(currentStackRangeRecord->isActive);
    currentStackRangeRecord->isActive = false;
    currentStackRangeRecord->endAddress = address;

    GarbageCollector::getCurrent().unregisterThisThread(currentStackRangeRecord);
}

void reactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(!currentStackRangeRecord->isActive);
    assert(currentStackRangeRecord->endAddress == address);
    GarbageCollector::getCurrent().registerThisThread();
    currentStackRangeRecord->isActive = true;
}

void gcSafePoint()
{
    assert(currentStackRangeRecord);
    GarbageCollector::getCurrent().safePoint();
}
} // End of namespace nyast
