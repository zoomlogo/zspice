#pragma once
#include "types.h"
#include "error.h"

// enum declaration
enum ctype {
#define COMPONENT(en, sn, av, p) en,
#include "component.def"
#undef COMPONENT
    _C_LEN
};

// individual component declaration
#define P(t, x) t x;
#define COMPONENT(en, sn, av, p) struct sn { p };
#include "component.def"
#undef COMPONENT
#undef P

// main component struct declaration
typedef struct {
    enum ctype type;

    // connections, proper node id
    // the real solver_id is id - 1
    // since id = 0 is ALWAYS ground
    usize id0; // generally the positive connection
    usize id1; // generally the negative connection

    // for multiple terminal devices
    usize id2;
    usize id3;

    // for components which require access to the C/B/D matrices
    usize solver_id;

    union {
#define COMPONENT(en, sn, av, p) struct sn av;
#include "component.def"
#undef COMPONENT
    };
} component_t;

component_t *new_component(enum ctype type);

// dc analysis: stamp functions
typedef error_e (*dc_stamp_f)(usize, f64 *, f64 *, component_t *);
#define COMPONENT(en, sn, av, p) error_e dc_stamp_##sn(usize dim, f64 *A, f64 *b, component_t *c);
#include "component.def"
#undef COMPONENT

// dc analysis: LUT
extern const dc_stamp_f DC_STAMPS[_C_LEN];

// ac analysis: stamp functions
typedef error_e (*ac_stamp_f)(usize, c64 *, c64 *, component_t *);
#define COMPONENT(en, sn, av, p) error_e ac_stamp_##sn(usize dim, c64 *A, c64 *b, component_t *c);
#include "component.def"
#undef COMPONENT

// ac analysis: LUT
extern const ac_stamp_f AC_STAMPS[_C_LEN];
