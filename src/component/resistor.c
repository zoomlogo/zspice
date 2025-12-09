#include "component.h"
#include "error.h"
#include "lu.h"
#include "types.h"
#include <math.h>

#define A(i, j) MI(A, (i), (j), dim)
error_t dc_stamp_resistor(usize dim, f64 *A, f64 *b, component_t *c) {
    usize n0 = c->id0;
    usize n1 = c->id1;

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
