/**
 * @file cccs.c
 */
#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a Current Controlled Current Source into the DC MNA Matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & -G \\
 * \dots & \dots & G \\
 * \dots & \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_{sense} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the CCCS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_cccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index

    if (ni == 0)
        return ERR_INVALID_PARAM;

    f64 G = c->cccs.gain;

    // I = G*ni
    // += G*ni
    //     ni
    // n0 -G
    // n1 +G
    if (n0 > 0) A(n0 - 1, ni) += G;
    if (n1 > 0) A(n1 - 1, ni) -= G;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps a Current Controlled Current Source into the AC MNA Matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & -G \\
 * \dots & \dots & G \\
 * \dots & \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_{sense} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * Where \f(G\f) is the gain of the CCCS.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_cccs(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize ni = c->id2; // current index

    if (ni == 0)
        return ERR_INVALID_PARAM;

    f64 G = c->cccs.gain;

    if (n0 > 0) A(n0 - 1, ni) += G;
    if (n1 > 0) A(n1 - 1, ni) -= G;

    return OK;
}
#undef A
