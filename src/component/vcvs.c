/**
 * @file vcvs.c
 */
#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a Voltage Controlled Voltage Source into the DC MNA Matrix.
 *
 * The MNA Matrix for the voltage source requires the knowledge of the
 * branch current through the component, thus introducing a row and column to the MNA matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & \dots & \dots & 1 \\
 * \dots & \dots & \dots & \dots & -1 \\
 * \dots & \dots & \dots & \dots & 0 \\
 * \dots & \dots & \dots & \dots & 0 \\
 * 1 & -1 & G & -G & 0
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ V_{n2} \\ V_{n3} \\ I_{src} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \\ 0 \\ V_{dc} \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the VCVS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_vcvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;
    usize nn = c->solver_id;


    f64 G = c->vcvs.gain;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    if (n2 > 0) A(nn, n2 - 1) -= G;
    if (n3 > 0) A(nn, n3 - 1) += G;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps a Voltage Controlled Voltage Source into the AC MNA Matrix.
 *
 * The MNA Matrix for the voltage source requires the knowledge of the
 * branch current through the component, thus introducing a row and column to the MNA matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & \dots & \dots & 1 \\
 * \dots & \dots & \dots & \dots & -1 \\
 * \dots & \dots & \dots & \dots & 0 \\
 * \dots & \dots & \dots & \dots & 0 \\
 * 1 & -1 & G & -G & 0
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ V_{n2} \\ V_{n3} \\ I_{src} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \\ 0 \\ V_{dc} \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the VCVS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_vcvs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize n2 = c->id2;
    usize n3 = c->id3;

    usize nn = c->solver_id;

    f64 G = c->vcvs.gain;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    if (n2 > 0) A(nn, n2 - 1) += G;
    if (n3 > 0) A(nn, n3 - 1) -= G;

    return OK;
}
#undef A
