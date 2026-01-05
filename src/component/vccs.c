#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_vccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    f64 G = c->vccs.gain;

    // I = G*(n2 - n3)
    // += G*(n2 - n3)
    //     n2  n3
    // n0 -G  +G
    // n1 +G  -G
    if (n0 > 0) {
        if (n2 > 0) A(n0 - 1, n2 - 1) -= G;
        if (n3 > 0) A(n0 - 1, n3 - 1) += G;
    }

    if (n1 > 0) {
        if (n2 > 0) A(n1 - 1, n2 - 1) += G;
        if (n3 > 0) A(n1 - 1, n3 - 1) -= G;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_vccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    f64 G = c->vccs.gain;

    if (n0 > 0) {
        if (n2 > 0) A(n0 - 1, n2 - 1) -= G;
        if (n3 > 0) A(n0 - 1, n3 - 1) += G;
    }

    if (n1 > 0) {
        if (n2 > 0) A(n1 - 1, n2 - 1) += G;
        if (n3 > 0) A(n1 - 1, n3 - 1) -= G;
    }

    return OK;
}
#undef A
