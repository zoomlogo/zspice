#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_cccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index

    if (ni == 0)
        return ERR_INVALID_PARAM;

    f64 G = c->cccs.gain;

    // I = G*ni
    // += G*ni
    //     ni
    // n0 -G
    // n1 +G
    if (n0 > 0) A(n0 - 1, ni) += G;
    if (n1 > 0) A(n1 - 1, ni) -= G;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_cccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index

    if (ni == 0)
        return ERR_INVALID_PARAM;

    f64 G = c->cccs.gain;

    if (n0 > 0) A(n0 - 1, ni) += G;
    if (n1 > 0) A(n1 - 1, ni) -= G;

    return OK;
}
#undef A
