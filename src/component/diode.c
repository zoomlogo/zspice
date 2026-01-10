/**
 * @file diode.c
 */
#include <math.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

error_e diode_linearize(component_t *c, env_t *env) {
    f64 V_D = c->D.Vj;
    if (isnan(c->D.V_T))
        c->D.V_T = env->V_T;
    f64 V_T = c->D.V_T;

    f64 exponential = exp(V_D / (c->D.N * V_T));
    f64 I_D = c->D.Is * (exponential - 1);
    f64 g_eq = (c->D.Is / c->D.N / V_T) * exponential;
    f64 i_eq = I_D - (g_eq * V_D);

    // save (for AC analysis)
    c->D.g_eq = g_eq;
    c->D.i_eq = I_D - (g_eq * V_D);
    return OK;
}

f64 diode_limit(component_t *c, f64 Vj) {
    if (isnan(c->D.Vcrit))
        c->D.Vcrit = c->D.N * c->D.V_T * log(sqrt(0.5) * c->D.N * c->D.V_T / c->D.Is);

    f64 Vcrit = c->D.Vcrit;
    if (Vj > Vcrit && Vj - c->D.Vj > 2 * c->D.V_T) {
        Vj = c->D.Vj + c->D.V_T * (1 + log((Vj - c->D.Vj) / c->D.V_T));
    }

    return Vj;
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_diode(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0; // anode
    usize n1 = c->id1; // cathode

    f64 g_eq = c->D.g_eq;
    f64 i_eq = c->D.i_eq;

    // stamp
    if (n0 > 0) {
        A(n0 - 1, n0 - 1) += g_eq;
        buf->b[n0 - 1] -= i_eq;
    }
    if (n1 > 0) {
        A(n1 - 1, n1 - 1) += g_eq;
        buf->b[n1  - 1] += i_eq;
    }

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= g_eq;
        A(n1 - 1, n0 - 1) -= g_eq;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_diode(sbuf_t *buf, component_t *c, env_t *env) {
    return ERR_UNIMPL;
}
#undef A
