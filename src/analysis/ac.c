#include <string.h>

#include "component/component.h"
#include "util/error.h"
#include "util/lu.h"

#include "analysis.h"
#include "types.h"

error_e ac_solve(circuit_t *circuit, f64 w) {
    // assuming circuit is linearized at this point...
    if (circuit->dim == 0) return ERR_NOT_INIT;

    c64 *A = (c64 *) circuit->A;
    c64 *b = (c64 *) circuit->b;

    // reset memory
    memset(A, 0, circuit->dim * circuit->dim * sizeof(c64));
    memset(b, 0, circuit->dim * sizeof(c64));

    // setup
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        error_e err = AC_STAMPS[c->type](circuit->dim, A, b, c, w);

        if (err != OK) return err;
    }

    // solve
    error_e err = c_lu_solve(A, circuit->dim, b);

    // copy mag+phase
    if (err != OK) return err;
    circuit->nodes[0].potential = 0;
    circuit->nodes[0].phase = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        c64 V = b[i - 1]; // AC phasor
        circuit->nodes[i].potential = cabs(V);
        circuit->nodes[i].phase = carg(V);
    }

    return OK;
}
