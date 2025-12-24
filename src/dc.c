#include <string.h>

#include "dc.h"
#include "circuit.h"
#include "component/component.h"
#include "error.h"
#include "lu.h"

error_e dc_solve_linear(circuit_t *circuit) {
    if (circuit->dim == 0) return ERR_NOT_INIT;

    f64 *A = (f64 *) circuit->A;
    f64 *b = (f64 *) circuit->b;

    // reset memory
    memset(A, 0, circuit->dim * circuit->dim * sizeof(f64));
    memset(b, 0, circuit->dim * sizeof(f64));

    // setup matrix
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        error_e err = DC_STAMPS[c->type](circuit->dim, A, b, c);

        if (err != OK) return err;
    }

    // solve
    error_e err = r_lu_solve(A, circuit->dim, b);

    // copy voltage values back into the nodes and cleanup
    if (err != OK) return err;
    circuit->nodes[0].potential = 0;
    circuit->nodes[0].phase = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        circuit->nodes[i].potential = b[i - 1];
        circuit->nodes[i].phase = 0;
    }

    return OK;
}

error_e dc_solve_non_linear(circuit_t *circuit) {
    return ERR_UNIMPL;
}
