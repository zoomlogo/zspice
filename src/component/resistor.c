/**
 * @file resistor.c
 */
#include <math.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps a resistor into the DC MNA Matrix.
 *
 * The MNA Matrix for the resistor is as follows:
 * \f[
 * \begin{bmatrix}
 * \frac 1R & -\frac 1R \\
 * -\frac 1R & \frac 1R \\
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * @note If `R.conductance` is `NAN`, it is automatically calculated
 *       from the resistance and stored to be used in the future.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_resistor(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (c->R.resistance == 0)
        return ERR_INVALID_PARAM;

    if (isnan(c->R.conductance))
        c->R.conductance = 1 / c->R.resistance;

    f64 G = c->R.conductance;

    if (n0 > 0) A(n0 - 1, n0 - 1) += G;
    if (n1 > 0) A(n1 - 1, n1 - 1) += G;

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= G;
        A(n1 - 1, n0 - 1) -= G;
    }

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps a resistor into the AC MNA Matrix.
 *
 * The MNA Matrix for the resistor is as follows:
 * \f[
 * \begin{bmatrix}
 * \frac 1R & -\frac 1R \\
 * -\frac 1R & \frac 1R \\
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \end{bmatrix}
 * \f]
 *
 * @note If `R.conductance` is `NAN`, it is automatically calculated
 *       from the resistance and stored to be used in the future.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_resistor(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (c->R.resistance == 0)
        return ERR_INVALID_PARAM;

    if (isnan(c->R.conductance))
        c->R.conductance = 1 / c->R.resistance;

    f64 G = c->R.conductance;

    if (n0 > 0) A(n0 - 1, n0 - 1) += G;
    if (n1 > 0) A(n1 - 1, n1 - 1) += G;

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= G;
        A(n1 - 1, n0 - 1) -= G;
    }

    return OK;
}
#undef A
