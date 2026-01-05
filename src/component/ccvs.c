#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_ccvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index
    usize nn = c->solver_id;

    if (ni == 0)
        return ERR_INVALID_PARAM;


    f64 G = c->ccvs.gain;

    // V = G*(n2)
    // b[nn] = Gni
    // ni
    // G  b[nn]
    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    A(nn, ni) += G;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_ccvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index
    usize nn = c->solver_id;

    if (ni == 0)
        return ERR_INVALID_PARAM;


    f64 G = c->ccvs.gain;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    A(nn, ni) += G;

    return OK;
}
#undef A
