/**
 * @file mosfet.c
 */
#include <math.h>
#include <stdio.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/log.h"
#include "util/lu.h"
#include "util/zmth.h"

#include "component.h"
#include "types.h"

error_e mosfet_linearize(component_t *c, env_t *env) {
    // Threshold voltage calculations:
    f64 Vbs = c->M.Vbs;
    f64 VT0 = c->M.VT0;
    f64 Vth;

    f64 dVth;
    if (Vbs <= 0) {
        Vth = VT0 - c->M.gamma * sqrt(2 * c->M.phi) + c->M.gamma * sqrt(2 * c->M.phi - Vbs);
        dVth = - c->M.gamma / (2 * sqrt(2 * c->M.phi - Vbs));
    } else if (Vbs <= 4 * c->M.phi) {
        Vth = VT0 - c->M.gamma * Vbs / sqrt(2 * c->M.phi);
        dVth = -c->M.gamma / sqrt(2 * c->M.phi);
    } else {
        Vth = VT0 - c->M.gamma * sqrt(2 * c->M.phi);
        dVth = 0;
    }

    f64 Vgs = c->M.Vgs;
    f64 Vds = c->M.Vds;
    f64 K = c->M.Kp * c->M.W / c->M.L;

    f64 sVds;
    if (Vds > 0) sVds = 1; else if (Vds < 0) sVds = -1; else sVds = 0;

    f64 Ids;
    f64 g_m;
    f64 g_ds;
    if (Vgs < Vth) {
        Ids = 0;
        g_m = 1e-12;
        g_ds = 1e-12;
    } else if (Vds < Vgs - Vth) {
        Ids = K * ((Vgs - Vth) * Vds - Vds * Vds / 2) * (1 + c->M.lambda * fabs(Vds));
        g_m = K * Vds * (1 + c->M.lambda * fabs(Vds));
        g_ds = K * ((Vgs - Vth) * Vds - Vds * Vds / 2) * c->M.lambda * sVds + K * (Vgs - Vth - Vds) * (1 + c->M.lambda * fabs(Vds));
    } else {
        Ids = K / 2 * (Vgs - Vth) * (Vgs - Vth) * (1 + c->M.lambda * fabs(Vds));
        g_m = K * (Vgs - Vth) * (1 + c->M.lambda * fabs(Vds));
        g_ds = K / 2 * (Vgs - Vth) * (Vgs - Vth) * c->M.lambda * sVds;
    }

    f64 g_mb = -g_m * dVth;

    // store
    c->M.Id = Ids;
    c->M.g_m = g_m;
    c->M.g_ds = g_ds;
    c->M.g_mb = g_mb;

    return OK;
}

void mosfet_limit(component_t *c, f64 Vgs, f64 Vds, f64 *r_Vgs, f64 *r_Vds) {
    *r_Vgs = zflimit(Vgs, c->M.Vgs, c->M.VT0);

    f64 Vgd = Vgs - Vds;
    f64 _Vgd = c->M.Vgs - c->M.Vds;
    Vgd = zflimit(Vgd, _Vgd, c->M.VT0);
    *r_Vds = *r_Vgs - Vgd;
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_mosfet(sbuf_t *buf, component_t *c, env_t *env) {
    usize ng = c->id0;
    usize nd = c->id1;
    usize ns = c->id2;
    usize nb = c->id3;

    f64 Ieq = c->M.Id - (c->M.g_m * c->M.Vgs + c->M.g_ds * c->M.Vds + c->M.g_mb * c->M.Vbs);

    if (nd > 0) {
        A(nd - 1, nd - 1) += c->M.g_ds;
        if (ng > 0) A(nd - 1, ng - 1) += c->M.g_m;
        if (ns > 0) A(nd - 1, ns - 1) -= (c->M.g_ds + c->M.g_m + c->M.g_mb);
        if (nb > 0) A(nd - 1, nb - 1) += c->M.g_mb;
    }

    if (ns > 0) {
        if (nd > 0) A(ns - 1, nd - 1) -= c->M.g_ds;
        if (ng > 0) A(ns - 1, ng - 1) -= c->M.g_m;
        A(ns - 1, ns - 1) += (c->M.g_ds + c->M.g_m + c->M.g_mb);
        if (nb > 0) A(ns - 1, nb - 1) -= c->M.g_mb;
    }

    if (nd > 0) buf->b[nd - 1] -= Ieq;
    if (ns > 0) buf->b[ns - 1] += Ieq;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_mosfet(sbuf_t *buf, component_t *c, env_t *env) {
    log_error("TODO");
    return ERR_UNIMPL;
}
#undef A
