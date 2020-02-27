#include "tests.h"

bool lexerTest(char* os);

#define CIMPLE_TESTS \
    CTEST(lexerTest)

#include <cimple.h>

bool lexerTest(char* os) {

    return false;
}

bool runTests() { return cimpleRunTests(); }
