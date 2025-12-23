#include <math.h>

#include "component.h"
#include "error.h"
#include "lu.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_t dc_stamp_current_source(usize dim, f64 *A, f64 *b, component_t *c) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    f64 I = c->I.current;

    if (n0 > 0) b[n0 - 1] += I;
    if (n1 > 0) b[n1 - 1] -= I;

    return OK;
}

error_t ac_stamp_current_source(usize dim, c64 *A, c64 *b, component_t *c) {
    return ERR_UNIMPL;
}
#undef A
