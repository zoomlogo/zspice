#pragma once

#define TESTS(X) \
    X(test_lu) \
    X(test_resistor) \
    X(test_voltage_source) \
    X(test_current_source) \
    X(test_circuit) \
    X(test_dc)


// define the functions
#define X(f) void f(void);
TESTS(X)
#undef X
