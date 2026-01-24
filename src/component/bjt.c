/**
 * @file bjt.c
 */
#include <math.h>
#include <stdio.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"
#include "util/zmth.h"

error_e bjt_linearize(component_t *c, env_t *env) {
    f64 Vbe = c->Q.Vbe; f64 Vbc = c->Q.Vbc;
    f64 Bf = c->Q.Bf; f64 Br = c->Q.Br;
    f64 Nf = c->Q.Nf; f64 Nr = c->Q.Nr;
    if (isnan(c->Q.V_T)) c->Q.V_T = env->V_T;
    f64 V_T = c->Q.V_T; f64 Is = c->Q.Is;

    // BE junction (diode)
    f64 eBE = exp(Vbe / (Nf * V_T));
    f64 I_cc = Is * (eBE - 1);
    f64 G_cc = Is / (Nf * V_T) * eBE;

    // BC junction (diode)
    f64 eBC = exp(Vbc / (Nr * V_T));
    f64 I_ec = Is * (eBC - 1);
    f64 G_ec = Is / (Nr * V_T) * eBC;

    // save (for AC analysis)
    c->Q.g_mf = G_cc;
    c->Q.g_mr = G_ec;
    c->Q.g_pi = c->Q.g_mf / Bf;
    c->Q.g_mu = c->Q.g_mr / Br;
    c->Q.g_o = 0; // TODO early effect

    c->Q.Ic = I_cc - I_ec - I_ec / Br;
    c->Q.Ib = I_cc / Bf + I_ec / Br;
    c->Q.Ie = -I_cc / Bf + I_ec - I_cc;

    return OK;
}

void bjt_limit(component_t *c, f64 Vbe, f64 Vbc, f64 *r_Vbe, f64 *r_Vbc) {
    if (isnan(c->Q.Vcritf))
        c->Q.Vcritf = c->Q.Nf * c->Q.V_T * log(sqrt(0.5) * c->Q.Nf * c->Q.V_T / c->Q.Is);
    if (isnan(c->Q.Vcritr))
        c->Q.Vcritr = c->Q.Nr * c->Q.V_T * log(sqrt(0.5) * c->Q.Nr * c->Q.V_T / c->Q.Is);

    *r_Vbe = zjlimit(Vbe, c->Q.Vbe, c->Q.V_T, c->Q.Vcritf);
    *r_Vbc = zjlimit(Vbc, c->Q.Vbc, c->Q.V_T, c->Q.Vcritr);
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_bjt(sbuf_t *buf, component_t *c, env_t *env) {
    usize nb = c->id0; // base
    usize ne = c->id1; // emitter
    usize nc = c->id2; // collector

    // stamp conductances
    if (nb > 0) {
        A(nb - 1, nb - 1) += c->Q.g_pi + c->Q.g_mu;
        if (nc > 0) A(nb - 1, ne - 1) += -c->Q.g_mu;
        if (ne > 0) A(nb - 1, ne - 1) += -c->Q.g_pi;
    }
    if (nc > 0) {
        if (nb > 0) A(nc - 1, nb - 1) += c->Q.g_mf - c->Q.g_mr - c->Q.g_mu;
        A(nc - 1, nc - 1) += c->Q.g_mr + c->Q.g_mu;
        if (ne > 0) A(nc - 1, ne - 1) += -c->Q.g_mf;
    }
    if (ne > 0) {
        if (nb > 0) A(ne - 1, nb - 1) += c->Q.g_mr - c->Q.g_mf - c->Q.g_pi;
        if (nc > 0) A(ne - 1, nc - 1) += -c->Q.g_mr;
        A(ne - 1, ne - 1) += c->Q.g_mf + c->Q.g_pi;
    }
    // stamp currents
    f64 vb = nb > 0 ? buf->b[nb - 1] : 0;
    f64 vc = nc > 0 ? buf->b[nc - 1] : 0;
    f64 ve = ne > 0 ? buf->b[ne - 1] : 0;
    f64 lIb = (c->Q.g_pi + c->Q.g_mu) * vb - c->Q.g_mu * vc - c->Q.g_pi * ve;
    f64 lIc = (c->Q.g_mf - c->Q.g_mr - c->Q.g_mu) * vb + (c->Q.g_mr + c->Q.g_mu) * vc - c->Q.g_mf * ve;
    f64 lIe = (c->Q.g_mr - c->Q.g_mf - c->Q.g_pi) * vb - c->Q.g_mr * vc + (c->Q.g_mf + c->Q.g_pi) * ve;

    if (nb > 0) buf->b[nb - 1] = lIb - c->Q.Ib;
    if (ne > 0) buf->b[ne - 1] = lIe - c->Q.Ie;
    if (nc > 0) buf->b[nc - 1] = lIc - c->Q.Ic;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_bjt(sbuf_t *buf, component_t *c, env_t *env) {
    return ERR_UNIMPL;
}
#undef A
