/**
 * @file diode.c
 */
#include <math.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"
#include "util/zmth.h"

/**
 * @brief Linearize a diode.
 *
 * A diode has the following parameters:
 * - \f(I_S\f), The saturation current.
 * - \f(N\f), The ideality factor.
 * - \f(V_D\f), The junction voltage.
 * - \f(V_b\f), The reverse breakdown voltage.
 *
 * We aim to linearize the diode by using its large signal model.
 * A large signal diode model consists of a current source, \f(I_{eq}\f) in parallel
 * with its DC conductance \f(g_{eq}\f). We account for reverse breakdown by considering
 * the diode to consist of "two" diodes following Shockley's equation.
 * \f[I_D = I_S(e^{\frac{V_D}{NV_T}} - 1) + I_Se^{-\frac{V_D + V_b}{NV_T}}.\f]
 *
 * Now we can calculate the equivalent large signal parameters:
 * \f[g_{eq} = \frac{I_S}{NV_T}\left(e^{\frac{V_D}{NV_T}} + e^{-\frac{V_D + V_b}{NV_T}}\right).\f]
 * \f[I_{eq} = I_D - g_{eq}V_D.\f]
 *
 * @param c The diode.
 * @param env The environment of the diode.
 * @returns OK on success.
 */
error_e diode_linearize(component_t *c, env_t *env) {
    f64 V_D = c->D.Vj;
    if (isnan(c->D.V_T))
        c->D.V_T = env->V_T;
    f64 V_T = c->D.V_T;

    f64 eforward = exp(V_D / (c->D.N * V_T));
    f64 ereverse = 0;
    if (c->D.V_break < 1e25)
        ereverse = exp(-(V_D + c->D.V_break) / (c->D.N * V_T));

    f64 I_D = c->D.Is * (eforward - 1) - (c->D.Is * ereverse);
    f64 g_eq = (c->D.Is / c->D.N / V_T) * (eforward + ereverse);
    f64 i_eq = I_D - (g_eq * V_D);

    // save (for AC analysis)
    c->D.g_eq = g_eq;
    c->D.i_eq = I_D - (g_eq * V_D);
    return OK;
}

/**
 * @brief Limit the next guess for the diode.
 *
 * This is important so that we don't accidentally jump so far into the
 * exponential such that the solver blows up to infinity. We compare
 * the new junction potential with a critical voltage, \f(V_C\f), above which we apply limiting.
 * This is also done on the breakdown side.
 *
 * The critical voltage is given by:
 * \f[V_C = NV_T\ln\left(\frac{NV_T}{2I_S}\right).\f]
 *
 * @param c The diode.
 * @param Vj The new diode voltage guess.
 * @returns OK on success.
 */
void diode_limit(component_t *c, f64 Vj, f64 *r_Vj) {
    if (isnan(c->D.Vcrit))
        c->D.Vcrit = c->D.N * c->D.V_T * log(sqrt(0.5) * c->D.N * c->D.V_T / c->D.Is);

    f64 V_break = c->D.V_break;
    if (Vj < -V_break || c->D.Vj < -V_break) // transform coords → limit → inverse transform coords
        *r_Vj = -(zjlimit(-(Vj + V_break), -(c->D.Vj + V_break), c->D.V_T, c->D.Vcrit) + V_break);
    else *r_Vj = zjlimit(Vj, c->D.Vj, c->D.V_T, c->D.Vcrit);
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a diode into the DC MNA Matrix.
 *
 * The MNA Matrix for a diode is as follows (assuming it is already linearized):
 * \f[
 * \begin{bmatrix}
 * g_{eq} & -g_{eq} \\
 * -g_{eq} & g_{eq}
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} -I_{dc} \\ I_{dc} \end{bmatrix}
 * \f]
 *
 * @note \f(V_{n0}\f) is the anode, \f(V_{n1}\f) is the cathode.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return OK on success.
 */
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
