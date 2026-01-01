#include <math.h>

#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_voltage_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    if (isnan(c->V.dc_offset))
        c->V.dc_offset = c->V.max_voltage;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    buf->b[nn] += c->V.dc_offset;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_voltage_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    if (isnan(c->V.dc_offset))
        c->V.dc_offset = 0;

    c64 V;
    if (isnan(c->V.frequency)) // sweep source
        V = c->V.max_voltage * cexp(J * M_PI * c->V.phase_offset / 180);
    else // fixed frequency source shorted
        V = 0;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    buf->zb[nn] += V;

    return OK;
}
#undef A
