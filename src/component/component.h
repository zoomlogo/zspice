#pragma once
#include "voltage_source.h"
#include "resistor.h"
#include "types.h"

#define ARR_MAX 32

enum ctype {
    RESISTOR, VOLTAGE_SOURCE
};

typedef struct {
    enum ctype type;

    usize id0; // positive connection
    usize id1; // negative connection

    union {
        struct resistor R;
        struct voltage_source V;
    };
} component_t;

component_t *new_component(enum ctype type);
