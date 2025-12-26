#include <math.h>

#include "util/error.h"
#include "util/lu.h"

#include "component.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_e dc_stamp_capacitor(usize dim, f64 *A, f64 *b, component_t *c) {
    // do nothing
    // as DC equivalent of capacitor is an open circuit
    return OK;
}

error_e ac_stamp_capacitor(usize dim, c64 *A, c64 *b, component_t *c, f64 w) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    // jwC
    c64 Y = J * w * c->C.capacitance;

    if (n0 > 0) A(n0 - 1, n0 - 1) += Y;
    if (n1 > 0) A(n1 - 1, n1 - 1) += Y;

    if (n0 > 0 && n1 > 0) {
        A(n0 - 1, n1 - 1) -= Y;
        A(n1 - 1, n0 - 1) -= Y;
    }

    return OK;
}
#undef A
