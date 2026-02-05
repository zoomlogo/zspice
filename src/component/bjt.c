/**
 * @file bjt.c
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

/**
 * @brief Linearize a BJT.
 *
 * A BJT has the following parameters:
 * - \f(I_S\f), The saturation current.
 * - \f(\beta_F\f), The forward junction gain.
 * - \f(\beta_R\f), The reverse junction gain.
 * - \f(N_F\f), The forward emission coefficient.
 * - \f(N_R\f), The reverse emission coefficient.
 * - \f(V_T\f), The thermal voltage.
 * - \f(V_A\f), The Early voltage.
 * - \f(C_{J0E}\f), The unbiased base-emitter junction capacitance.
 * - \f(C_{J0C}\f), The unbiased base-collector junction capacitance.
 * - \f(m_E\f), The base-emitter capacitance gradient factor.
 * - \f(m_C\f), The base-collector capacitance gradient factor.
 * - \f(\phi_E\f), The base-emitter barrier potential.
 * - \f(\phi_C\f), The base-collector barrier potential.
 * - \f(\tau_F\f), The forward transit time of charges.
 * - \f(\tau_R\f), The reverse transit time of charges.
 * - \f(V_{BE}\f), The potential drop across the base-emitter junction.
 * - \f(V_{BC}\f), The potential drop across the base-collector junction.
 *
 * We aim to linearize the BJT using its large signal model. The model we are using here is a
 * slightly modified EM2 model (as described in "Modelling the Bipolar Transistor, Ian E. Getreu").
 * The modifications are as follows:
 * - The three terminal resistors are not modelled here, since they require internal nodes, which
 *   can be easily handled by the parser.
 * - Modelling of Early effect has been added.
 *
 * Now the model is as follows, EM2 model is a simple modification of the normal
 * Ebers-Moll model (injection version) by adding capacitances. The transistor
 * behaves as two _dependent_ pn-junctions. For the BE junction, we have:
 * \f[I_{CC} = I_S\left(e^{\frac{V_{BE}}{N_FV_T}} - 1\right).\f]
 * \f[G_{CC} = \frac{I_S}{N_FV_T}e^{\frac{V_{BE}}{N_FV_T}}.\f]
 *
 * Similarily for the BC junction:
 * \f[I_{EC} = I_S\left(e^{\frac{V_{BC}}{N_RV_T}} - 1\right).\f]
 * \f[G_{EC} = \frac{I_S}{N_RV_T}e^{\frac{V_{BC}}{N_RV_T}}.\f]
 *
 * To model Early effect, we compute the Early effect factor (this is an approximation):
 * \f[f_{EE} = 1 + \frac{V_{BE}-V_{BC}}{V_A}.\f]
 *
 * The injection model transport current, which captures the dependency of the junctions, is then given by:
 * \f[I_{CT} = f_{EE}\left(I_{CC} - I_{EC}\right).\f]
 *
 * We can then compute the terminal currents \f(I_C\f), \f(I_B\f) and \f(I_E\f) as follows:
 * \f[I_C = I_{CT} - \frac{I_{EC}}{\beta_R}.\f]
 * \f[I_B = \frac{I_{CC}}{\beta_F} + \frac{I_{EC}}{\beta_R}.\f]
 * \f[I_E = -I_{CT} - \frac{I_{CC}}{\beta_F}.\f]
 * We take convention that current flowing _into_ the terminals are positive.
 *
 * Finally we compute the equivalent conductances which are stored:
 * \f[g_o = \frac{I_{CC} - I_{EC}}{V_A}.\f]
 * \f[g_{mf} = f_{EE}G_{CC}.\f]
 * \f[g_{mr} = f_{EE}G_{EC}.\f]
 * \f[g_{\pi} = \frac{G_{CC}}{\beta_F}.\f]
 * \f[g_{\mu} = \frac{G_{EC}}{\beta_R}.\f]
 *
 * Now to model the bandwidth of the transistor correctly we need to add the junction capacitances. These
 * are voltage dependent parameters which are modelled as follows:
 * \f[C_{JE} = \frac{C_{JE0}}{\left(1 - \frac{V_{BE}}{\phi_E}\right)^{m_E}}.\f]
 * \f[C_{JC} = \frac{C_{JC0}}{\left(1 - \frac{V_{BC}}{\phi_C}\right)^{m_C}}.\f]
 * However because of the singularity present when \f(V_{BE} = \phi_E\f) (or when \f(V_{BC} = \phi_C\f)) we
 * switch to a linear approximation of the formula (when \f(V > \frac 12\phi\f)):
 * \f[C_{JE} = C_{JE0}\times\left(\frac 12\right)^{-\left(1 + m_E\right)}\times\left(\frac 12 - \frac 12m_E + \frac{m_EV_{BE}}{\phi_E}\right).\f]
 * \f[C_{JC} = C_{JC0}\times\left(\frac 12\right)^{-\left(1 + m_C\right)}\times\left(\frac 12 - \frac 12m_C + \frac{m_CV_{BC}}{\phi_C}\right).\f]
 *
 * To model the diffusion capacitances, we use the following formulae:
 * \f[C_{DE} = \tau_Fg_{mf}.\f]
 * \f[C_{DC} = \tau_Rg_{mr}.\f]
 *
 * Finally the net capacitance is stored:
* \f[C_{BE} = C_{JE} + C_{DE}.\f]
* \f[C_{BC} = C_{JC} + C_{DC}.\f]
*
* @param c The BJT.
* @param env The environment of the BJT.
* @returns OK on success.
 */
error_e bjt_linearize(component_t *c, env_t *env) {
    f64 Vbe = c->Q.Vbe; f64 Vbc = c->Q.Vbc;
    f64 Bf = c->Q.Bf; f64 Br = c->Q.Br;
    f64 Nf = c->Q.Nf; f64 Nr = c->Q.Nr;
    if (isnan(c->Q.V_T)) c->Q.V_T = env->V_T;
    f64 V_T = c->Q.V_T; f64 Is = c->Q.Is;
    f64 Va = c->Q.Va;

    // BE junction (diode)
    f64 eBE = exp(Vbe / (Nf * V_T));
    f64 I_cc = Is * (eBE - 1);
    f64 G_cc = Is / (Nf * V_T) * eBE;

    // BC junction (diode)
    f64 eBC = exp(Vbc / (Nr * V_T));
    f64 I_ec = Is * (eBC - 1);
    f64 G_ec = Is / (Nr * V_T) * eBC;

    // early effect
    f64 ee = 1 + (Vbe - Vbc) / Va; ee = zclamp(ee, 0.0001, INFINITY);

    // transport current
    f64 I_ct = (I_cc - I_ec) * ee;

    // compute junction capacitances
    f64 Cje;
    f64 Cjc;
    if (Vbe > 0.5 * c->Q.phi_e) {
        Cje = c->Q.Cj0e * pow(0.5, -(1 + c->Q.m_e)) * (0.5 - 0.5 * c->Q.m_e + c->Q.m_e * c->Q.Vbe / c->Q.phi_e);
    } else
        Cje = c->Q.Cj0e / pow(1 - Vbe / c->Q.phi_e, c->Q.m_e);
    if (Vbc > 0.5 * c->Q.phi_c) {
        Cjc = c->Q.Cj0c * pow(0.5, -(1 + c->Q.m_c)) * (0.5 - 0.5 * c->Q.m_c + c->Q.m_c * c->Q.Vbc / c->Q.phi_c);
    } else
        Cjc = c->Q.Cj0c / pow(1 - Vbc / c->Q.phi_c, c->Q.m_c);

    // compute (dc) terminal currents
    c->Q.Ic = I_ct - I_ec / Br;
    c->Q.Ib = I_cc / Bf + I_ec / Br;
    c->Q.Ie = -I_cc / Bf - I_ct;

    // save (for AC analysis)
    c->Q.g_o = (I_cc - I_ec) / Va;
    c->Q.g_mf = G_cc * ee;
    c->Q.g_mr = G_ec * ee;
    c->Q.g_pi = G_cc / Bf;
    c->Q.g_mu = G_ec / Br;

    // compute diffusion capacitances
    f64 Cde = c->Q.tau_f * c->Q.g_mf;
    f64 Cdc = c->Q.tau_r * c->Q.g_mr;

    c->Q.c_be = Cje + Cde;
    c->Q.c_bc = Cjc + Cdc;

    return OK;
}

/**
 * @brief Limit the next guesses for the BJT.
 *
 * This is the exact same logic as the diode_limit() function. We just
 * apply it twice, once to each pn junction of the BJT.
 *
 * @param c The BJT.
 * @param Vbe The new base-emitter voltage guess.
 * @param Vbc The new base-collector voltage guess.
 * @param r_Vbe (Output) The limited base-emitter voltage.
 * @param r_Vbc (Output) The limited base-collector voltage.
 */
void bjt_limit(component_t *c, f64 Vbe, f64 Vbc, f64 *r_Vbe, f64 *r_Vbc) {
    if (isnan(c->Q.Vcritf))
        c->Q.Vcritf = c->Q.Nf * c->Q.V_T * log(sqrt(0.5) * c->Q.Nf * c->Q.V_T / c->Q.Is);
    if (isnan(c->Q.Vcritr))
        c->Q.Vcritr = c->Q.Nr * c->Q.V_T * log(sqrt(0.5) * c->Q.Nr * c->Q.V_T / c->Q.Is);

    *r_Vbe = zjlimit(Vbe, c->Q.Vbe, c->Q.V_T, c->Q.Vcritf);
    *r_Vbc = zjlimit(Vbc, c->Q.Vbc, c->Q.V_T, c->Q.Vcritr);
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a BJT into the DC MNA matrix.
 *
 * The MNA Matrix for a BJT is as follows (assuming it is already linearized):
 * \f[
 * \begin{bmatrix}
 * g_{\pi} + g_{\mu} & -g_{\mu} & -g_{\pi} \\
 * g_{mf} - g_{mr} - g_{\mu} & g_{mr} + g_{\mu} + g_o & -g_{mf} \\
 * g_{mr} - g_{mf} - g_{\pi} & -\left(g_{mr} + g_o\right) & g_{mf} + g_{\pi} + g_o
 * \end{bmatrix}
 * \begin{bmatrix} V_B \\ V_C \\ V_E \end{bmatrix}
 * =
 * \begin{bmatrix}
 * g_{\pi}V_{BE} + g_{\mu}V_{BC} - I_B \\
 * g_{mf}V_{BE} - \left(g_{\mu} + g_{mr}\right)V_{BC} - I_E \\
 * -\left(g_{mf} + g_{\pi}\right)V_{BE} + g_{mr}V_{BC} - I_C
 * \end{bmatrix}
 * \f]
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return OK on success.
 */
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
        A(nc - 1, nc - 1) += c->Q.g_mr + c->Q.g_mu + c->Q.g_o;
        if (ne > 0) A(nc - 1, ne - 1) += -c->Q.g_mf;
    }
    if (ne > 0) {
        if (nb > 0) A(ne - 1, nb - 1) += c->Q.g_mr - c->Q.g_mf - c->Q.g_pi;
        if (nc > 0) A(ne - 1, nc - 1) += -c->Q.g_mr - c->Q.g_o;
        A(ne - 1, ne - 1) += c->Q.g_mf + c->Q.g_pi + c->Q.g_o;
    }
    // stamp currents
    f64 lIb = (c->Q.g_pi * c->Q.Vbe) + (c->Q.g_mu * c->Q.Vbc);
    f64 lIc = c->Q.g_mf * c->Q.Vbe - (c->Q.g_mu + c->Q.g_mr) * c->Q.Vbc;
    f64 lIe = -(c->Q.g_mf + c->Q.g_pi) * c->Q.Vbe + c->Q.g_mr * c->Q.Vbc;

    if (nb > 0) buf->b[nb - 1] += lIb - c->Q.Ib;
    if (ne > 0) buf->b[ne - 1] += lIe - c->Q.Ie;
    if (nc > 0) buf->b[nc - 1] += lIc - c->Q.Ic;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_bjt(sbuf_t *buf, component_t *c, env_t *env) {
    log_error("TODO");
    return ERR_UNIMPL;
}
#undef A
