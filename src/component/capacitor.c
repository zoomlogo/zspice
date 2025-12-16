#include <math.h>

#include "component.h"
#include "error.h"
#include "lu.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_t dc_stamp_capacitor(usize dim, f64 *A, f64 *b, component_t *c) {
    // do nothing
    // as DC equivalent of capacitor is an open circuit
    return OK;
}
#undef A
