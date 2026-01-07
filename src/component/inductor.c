/**
 * @file inductor.c
 */
#include <math.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps an inductor into the DC MNA Matrix.
 *
 * The MNA Matrix for an inductor requires the knowledge of the
 * branch current through it, thus introducing a row and column to the MNA matrix.
 * The DC equivalent of an inductor is a short circuit, which is the same as a
 * 0V voltage source.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & 1 \\
 * \dots & \dots & -1 \\
 * 1 & -1 & 0
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_L \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_inductor(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;
    // dc equivalent of inductor is short (or a 0V voltage source :P)

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps an inductor into the AC MNA Matrix.
 *
 * The MNA Matrix for an inductor requires the knowledge of the
 * branch current through it, thus introducing a row and column to the MNA matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & 1 \\
 * \dots & \dots & -1 \\
 * 1 & -1 & -j\omega L
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_L \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_inductor(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    c64 Z = J * env->w * c->L.inductance;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    A(nn, nn) -= Z;

    return OK;
}
#undef A
