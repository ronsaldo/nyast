#include "UnitTest++/UnitTest++.h"
#include "nyast/Oop.hpp"

int main(int, char const *[])
{
    return nyast::withGarbageCollectedStack([]() {
        return UnitTest::RunAllTests();
    });
}
