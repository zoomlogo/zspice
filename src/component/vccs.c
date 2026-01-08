/**
 * @file vccs.c
 */
#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a Voltage Controlled Current Source into the DC MNA Matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & -G & G \\
 * \dots & \dots & G & -G \\
 * \dots & \dots & \dots & \dots \\
 * \dots & \dots & \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ V_{n2} \\ V_{n3} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the VCCS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return OK on success.
 */
error_e dc_stamp_vccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    f64 G = c->vccs.gain;

    if (n0 > 0) {
        if (n2 > 0) A(n0 - 1, n2 - 1) -= G;
        if (n3 > 0) A(n0 - 1, n3 - 1) += G;
    }

    if (n1 > 0) {
        if (n2 > 0) A(n1 - 1, n2 - 1) += G;
        if (n3 > 0) A(n1 - 1, n3 - 1) -= G;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps a Voltage Controlled Current Source into the AC MNA Matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & -G & G \\
 * \dots & \dots & G & -G \\
 * \dots & \dots & \dots & \dots \\
 * \dots & \dots & \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ V_{n2} \\ V_{n3} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the VCCS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return OK on success.
 */
error_e ac_stamp_vccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    f64 G = c->vccs.gain;

    if (n0 > 0) {
        if (n2 > 0) A(n0 - 1, n2 - 1) -= G;
        if (n3 > 0) A(n0 - 1, n3 - 1) += G;
    }

    if (n1 > 0) {
        if (n2 > 0) A(n1 - 1, n2 - 1) += G;
        if (n3 > 0) A(n1 - 1, n3 - 1) -= G;
    }

    return OK;
}
#undef A
