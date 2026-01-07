/**
 * @file voltage_source.c
 */
#include <math.h>

#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps an Independent Voltage Source into the DC MNA Matrix.
 *
 * The MNA Matrix for an independent voltage source requires the knowledge of the
 * branch current through the component, thus introducing a row and column to the MNA matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & 1 \\
 * \dots & \dots & -1 \\
 * 1 & -1 & 0
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_{src} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ V_{dc} \end{bmatrix}
 * \f]
 *
 * @note If `V.dc_offset` is `NAN`, it is automatically set to `V.max_voltage`. `dc_offset`
 *       is used in DC analysis, while `max_voltage` is ignored.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_voltage_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    if (isnan(c->V.dc_offset))
        c->V.dc_offset = c->V.max_voltage;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    buf->b[nn] += c->V.dc_offset;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps an Independent Voltage Source into the AC MNA Matrix.
 *
 * The MNA Matrix for an independent voltage source requires the knowledge of the
 * branch current through the component, thus introducing a row and column to the MNA matrix.
 *
 * The MNA Matrix is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots & 1 \\
 * \dots & \dots & -1 \\
 * 1 & -1 & 0
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \\ I_{src} \end{bmatrix}
 * =
 * \begin{bmatrix} 0 \\ 0 \\ V_{max}e^{j\pi\phi} \end{bmatrix}
 * \f]
 *
 * Note that \f(\phi\f) is the phase offset of the source in radians.
 *
 * @note If `V.dc_offset` is `NAN`, it is automatically set to `0`.
 *       Sources with `V.frequency` set to `NAN` are considered for analysis
 *       and if the frequency is set they are shorted.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_voltage_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    if (isnan(c->V.dc_offset))
        c->V.dc_offset = 0;

    c64 V;
    if (isnan(c->V.frequency)) // sweep source
        V = c->V.max_voltage * cexp(J * M_PI * c->V.phase_offset / 180);
    else // fixed frequency source shorted
        V = 0;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    buf->zb[nn] += V;

    return OK;
}
#undef A
