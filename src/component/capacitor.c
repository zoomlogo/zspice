/**
 * @file capacitor.c
 */
#include <math.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a capacitor into the DC MNA Matrix.
 *
 * The DC equivalent of a capacitor is an open circuit, so
 * we do not need to stamp anything.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_capacitor(sbuf_t *buf, component_t *c, env_t *env) {
    // do nothing
    // as DC equivalent of capacitor is an open circuit
    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps a capacitor into the AC MNA Matrix.
 *
 * The MNA Matrix for the capacitor is as follows:
 * \f[
 * \begin{bmatrix}
 * j\omega C & -j\omega C \\
 * -j\omega C & j\omega C \\
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_capacitor(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    // jwC
    c64 Y = J * env->w * c->C.capacitance;

    if (n0 > 0) A(n0 - 1, n0 - 1) += Y;
    if (n1 > 0) A(n1 - 1, n1 - 1) += Y;

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= Y;
        A(n1 - 1, n0 - 1) -= Y;
    }

    return OK;
}
#undef A
