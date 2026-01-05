#pragma once
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "types.h"

// enum declaration
enum ctype {
#define COMPONENT(en, sn, av, p) en,
#include "component.def"
#undef COMPONENT
    _C_LEN
};

// individual component declaration
#define P(t, x, d) t x;
#define COMPONENT(en, sn, av, p) struct sn { p };
#include "component.def"
#undef COMPONENT
#undef P

// main component struct declaration
typedef struct {
    enum ctype type;

    // Set node with id = 0 as our reference (ground).
    // Hence the real solver_id is id - 1.
    usize id0; // +
    usize id1; // -
    // Two terminal components: id0 +, id1 -.

    // Used for multiple terminal devices.
    usize id2;
    usize id3;
    // For VCCS/VCVS: id2 +, id3 -.
    // For CCCS/CCVS: id2 node denoting solver id of 0 V source

    // For three terminal components:
    // TODO

    // For components which require access to the C/B/D matrices,
    // i.e. they introduce an unknown branch current into the MNA matrix.
    usize solver_id;

    union {
#define COMPONENT(en, sn, av, p) struct sn av;
#include "component.def"
#undef COMPONENT
    };
} component_t;

component_t *new_component(enum ctype type);

// dc analysis: stamp functions
typedef error_e (*dc_stamp_f)(sbuf_t *, component_t *, env_t *);
#define COMPONENT(en, sn, av, p) error_e dc_stamp_##sn(sbuf_t *buf, component_t *c, env_t *env);
#include "component.def"
#undef COMPONENT

// dc analysis: LUT
extern const dc_stamp_f DC_STAMPS[_C_LEN];

// ac analysis: stamp functions
typedef error_e (*ac_stamp_f)(sbuf_t *, component_t *, env_t *);
#define COMPONENT(en, sn, av, p) error_e ac_stamp_##sn(sbuf_t *buf, component_t *c, env_t *env);
#include "component.def"
#undef COMPONENT

// ac analysis: LUT
extern const ac_stamp_f AC_STAMPS[_C_LEN];

// set defaults
void c_defaults(component_t *component);
