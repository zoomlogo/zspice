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
    f64 Vbe = c->Q.Vbe;
    f64 Vbc = c->Q.Vbc;
    if (isnan(c->Q.V_T))
        c->Q.V_T = env->V_T;
    f64 V_T = c->Q.V_T;
    f64 Is = c->Q.Is;

    // alpha = beta / (1 + beta)
    f64 af = c->Q.Bf / (1 + c->Q.Bf);
    f64 ar = c->Q.Br / (1 + c->Q.Br);

    // BE junction (diode)
    f64 eBE = exp(Vbe / (c->Q.Nf * V_T));
    f64 I_be = Is * (eBE - 1);
    f64 G_be = Is / (c->Q.Nf * V_T) * eBE;
    c->Q.i_be = I_be - G_be * Vbe;

    // BC junction (diode)
    f64 eBC = exp(Vbc / (c->Q.Nr * V_T));
    f64 I_bc = Is * (eBC - 1);
    f64 G_bc = Is / (c->Q.Nr * V_T) * eBC;
    c->Q.i_bc = I_bc - G_bc * Vbc;

    // save (for AC analysis)
    c->Q.g_pi = G_be * (1 - af);
    c->Q.g_mu = G_bc * (1 - ar);
    c->Q.g_mf = G_be * af;
    c->Q.g_mr = G_bc * ar;
    c->Q.g_o = 0; // TODO

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
    // TODO why does this work? HOW does this work?
    usize n0 = c->id0; // base
    usize n1 = c->id1; // emitter
    usize n2 = c->id2; // collector

    f64 af = c->Q.Bf / (1 + c->Q.Bf);
    f64 ar = c->Q.Br / (1 + c->Q.Br);

    // g_pi, g_mu, g_o
    if (n0 > 0) {
        A(n0 - 1, n0 - 1) += c->Q.g_pi + c->Q.g_mu;
        buf->b[n0 - 1] -= c->Q.i_bc + c->Q.i_be;
    }

    if (n1 > 0) {
        A(n1 - 1, n1 - 1) += c->Q.g_pi + c->Q.g_o;
        buf->b[n1 - 1] += c->Q.i_be * (1 + af) - c->Q.i_bc * ar;
    }

    if (n2 > 0) {
        A(n2 - 1, n2 - 1) += c->Q.g_mu + c->Q.g_o;
        buf->b[n2 - 1] += c->Q.i_bc * (1 + ar) - c->Q.i_be * af;
    }

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= c->Q.g_pi;
        A(n1 - 1, n0 - 1) -= c->Q.g_pi;
    }

    if (n0 > 0 && n2 > 0) {
        A(n0 - 1, n2 - 1) -= c->Q.g_mu;
        A(n2 - 1, n0 - 1) -= c->Q.g_mu;
    }

    if (n1 > 0 && n2 > 0) {
        A(n1 - 1, n2 - 1) -= c->Q.g_o;
        A(n2 - 1, n1 - 1) -= c->Q.g_o;
    }

    // g_mf C→E, Vbe; g_mr E→C, Vbc
    if (n1 > 0) {
        A(n1 - 1, n1 - 1) += c->Q.g_mf;
        if (n0 > 0) A(n1 - 1, n0 - 1) += c->Q.g_mr - c->Q.g_mf;
        if (n2 > 0) A(n1 - 1, n2 - 1) -= c->Q.g_mr;
    }

    if (n2 > 0) {
        A(n2 - 1, n2 - 1) += c->Q.g_mr;
        if (n0 > 0) A(n2 - 1, n0 - 1) += c->Q.g_mf - c->Q.g_mr;
        if (n1 > 0) A(n2 - 1, n1 - 1) -= c->Q.g_mf;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_bjt(sbuf_t *buf, component_t *c, env_t *env) {
    return ERR_UNIMPL;
}
#undef A
