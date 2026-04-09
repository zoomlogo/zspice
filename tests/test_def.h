#pragma once

#define TESTS(X)          \
        X(lu)             \
        X(csv)            \
        X(resistor)       \
        X(capacitor)      \
        X(inductor)       \
        X(voltage_source) \
        X(current_source) \
        X(diode)          \
        X(bjt)            \
        X(circuit)        \
        X(dc)             \
        X(ac)

// define the functions
#define X(f) void test_##f(void);
TESTS(X)
#undef X
