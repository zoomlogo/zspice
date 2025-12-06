#pragma once
#include "types.h"

// enum declaration
enum ctype {
#define COMPONENT(en, sn, av, p) en,
#include "component.def"
#undef COMPONENT
};

// individual component declaration
#define P(x) f64 x;
#define COMPONENT(en, sn, av, p) struct sn { p };
#include "component.def"
#undef COMPONENT
#undef P

// main component struct declaration
typedef struct {
    enum ctype type;

    usize id0; // positive connection
    usize id1; // negative connection

    union {
#define COMPONENT(en, sn, av, p) struct sn av;
#include "component.def"
#undef COMPONENT
    };
} component_t;

component_t *new_component(enum ctype type);
