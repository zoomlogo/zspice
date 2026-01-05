#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_vcvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;
    usize nn = c->solver_id;


    f64 G = c->vcvs.gain;

    // V = G*(n2 - n3)
    // b[nn] = Gn2 - Gn3
    // n2 n3
    // G  -G  b[nn]
    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    if (n2 > 0) A(nn, n2 - 1) -= G;
    if (n3 > 0) A(nn, n3 - 1) += G;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_vcvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    usize nn = c->solver_id;

    f64 G = c->vcvs.gain;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    if (n2 > 0) A(nn, n2 - 1) += G;
    if (n3 > 0) A(nn, n3 - 1) -= G;

    return OK;
}
#undef A
