#include <math.h>

#include "component.h"
#include "error.h"
#include "lu.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_t dc_stamp_capacitor(usize dim, f64 *A, f64 *b, component_t *c) {
    usize n0 = c->id0;
    usize n1 = c->id1;

    return ERR_UNIMPL;
}
#undef A
