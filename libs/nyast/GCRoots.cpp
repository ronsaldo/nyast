#include "nyast/Oop.hpp"
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
    (void)rootCount;
    (void)roots;
}

void unregisterGlobalOopRoots(size_t rootCount, Oop *roots)
{
    (void)rootCount;
    (void)roots;
}

void activateStackRangeRecord(StackRangeRecord *record)
{
    record->isActive = true;
    record->previous = currentStackRangeRecord;
    currentStackRangeRecord = record;
}

void deactivateStackRangeRecord(StackRangeRecord *record)
{
    record->isActive = false;
    currentStackRangeRecord = record->previous;
}

void deactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(currentStackRangeRecord->isActive);
    currentStackRangeRecord->isActive = false;
    currentStackRangeRecord->endAddress = address;
}

void reactivateCurrentStackRangeRecordAtAddress(const void *address)
{
    assert(currentStackRangeRecord != nullptr);
    assert(!currentStackRangeRecord->isActive);
    assert(currentStackRangeRecord->endAddress == address);
    currentStackRangeRecord->isActive = true;
}

void gcSafePoint()
{
    assert(currentStackRangeRecord);
}
} // End of namespace nyast
