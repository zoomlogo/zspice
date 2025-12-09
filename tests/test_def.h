#pragma once

#define TESTS(X) \
    X(test_lu) \
    X(test_circuit) \
    X(test_resistor)


// define the functions
#define X(f) void f(void);
TESTS(X)
#undef X
