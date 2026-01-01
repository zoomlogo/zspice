#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_e dc_stamp_current_source(usize dim, f64 *A, f64 *b, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (isnan(c->I.dc_offset))
        c->I.dc_offset = c->I.max_current;

    f64 I = c->I.dc_offset;

    if (n0 > 0) b[n0 - 1] += I;
    if (n1 > 0) b[n1 - 1] -= I;

    return OK;
}

error_e ac_stamp_current_source(usize dim, c64 *A, c64 *b, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (isnan(c->I.dc_offset))
        c->I.dc_offset = 0;

    c64 I;
    if (isnan(c->I.frequency)) // sweep source
        I = c->I.max_current * cexp(J * M_PI * c->I.phase_offset / 180);
    else // fixed frequency source shorted
        I = 0;

    if (n0 > 0) b[n0 - 1] += I;
    if (n1 > 0) b[n1 - 1] -= I;

    return OK;
}
#undef A
