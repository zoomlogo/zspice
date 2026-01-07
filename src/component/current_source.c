/**
 * @file current_source.c
 */
#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
/**
 * @brief Stamps an Independent Current Source into the DC MNA Matrix.
 *
 * The MNA Matrix for an independent current source is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots \\
 * \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} I_{dc} \\ -I_{dc} \end{bmatrix}
 * \f]
 *
 * @note If `I.dc_offset` is `NAN`, it is automatically set to `I.max_voltage`. `dc_offset`
 *       is used in DC analysis, while `max_voltage` is ignored.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e dc_stamp_current_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (isnan(c->I.dc_offset))
        c->I.dc_offset = c->I.max_current;

    f64 I = c->I.dc_offset;

    if (n0 > 0) buf->b[n0 - 1] += I;
    if (n1 > 0) buf->b[n1 - 1] -= I;

    return OK;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
/**
 * @brief Stamps an Independent Current Source into the AC MNA Matrix.
 *
 * The MNA Matrix for an independent current source is as follows:
 * \f[
 * \begin{bmatrix}
 * \dots & \dots \\
 * \dots & \dots
 * \end{bmatrix}
 * \begin{bmatrix} V_{n0} \\ V_{n1} \end{bmatrix}
 * =
 * \begin{bmatrix} I_{max}e^{j\pi\phi} \\ -I_{max}e^{j\pi\phi} \end{bmatrix}
 * \f]
 *
 * Note that \f(\phi\f) is the phase offset of the source in radians.
 *
 * @note If `I.dc_offset` is `NAN`, it is automatically set to `0`.
 *       Sources with `I.frequency` set to `NAN` are considered for analysis
 *       and if the frequency is set they are opened.
 *
 * @param buf The solver buffer.
 * @param c Pointer to the component.
 * @param env Simulation environment.
 * @return error_e OK on success.
 */
error_e ac_stamp_current_source(sbuf_t *buf, component_t *c, env_t *env) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    if (isnan(c->I.dc_offset))
        c->I.dc_offset = 0;

    c64 I;
    if (isnan(c->I.frequency)) // sweep source
        I = c->I.max_current * cexp(J * M_PI * c->I.phase_offset / 180);
    else // fixed frequency source shorted
        I = 0;

    if (n0 > 0) buf->zb[n0 - 1] += I;
    if (n1 > 0) buf->zb[n1 - 1] -= I;

    return OK;
}
#undef A
