#pragma once
#include "types.h"

#define ARR_MAX 32

enum ctype {
    RESISTOR, VOLTAGE_SOURCE
};

typedef struct {
    enum ctype type;
    f64 properties[ARR_MAX];
} component_t;

component_t *new_component(enum ctype type);
void p_set(component_t *component, char *p, f64 v);
f64 p_get(component_t *component, char *p);
