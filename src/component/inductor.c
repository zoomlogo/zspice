#include <math.h>

#include "component.h"
#include "error.h"
#include "lu.h"
#include "types.h"

#define A(i, j) MI(A, (i), (j), dim)
error_e dc_stamp_inductor(usize dim, f64 *A, f64 *b, component_t *c) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;
    // dc equivalent of inductor is short (or a 0V voltage source :P)

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    return OK;
}

error_e ac_stamp_inductor(usize dim, c64 *A, c64 *b, component_t *c, f64 w) {
    usize n0 = c->id0;
    usize n1 = c->id1;
    usize nn = c->solver_id;

    c64 Z = J * w * c->L.inductance;

    if (n0 > 0) {
        A(n0 - 1, nn) += 1;
        A(nn, n0 - 1) += 1;
    }

    if (n1 > 0) {
        A(n1 - 1, nn) -= 1;
        A(nn, n1 - 1) -= 1;
    }

    A(nn, nn) += Z;

    return OK;
}
#undef A

/*
inductor: L
AC: j⍵L

suppose this circuit
+----88888-->-+
|      L   I2 |
|<V1          |
|     +V-  I1 |
+-----| :--<--+

V = I * j⍵L

[  0  1    -1 ][ V1 ] = [  0 ]
[  1  0     0 ][ I1 ] = [ ~V ]
[ -1  0   j⍵L ][ I2 ] = [  0 ]

I1 - I2 = 0        ...1
V1 = ~V            ...2
-V1 + j⍵L×I2 = 0   ...3
*/
