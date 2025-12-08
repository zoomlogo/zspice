#include <string.h>

#include "dc.h"
#include "component/component.h"
#include "error.h"
#include "lu.h"

error_t dc_solve(circuit_t *circuit) {
    if (circuit->dim == 0) return ERR_NOT_INIT;

    // reset memory
    memset(circuit->A, 0, circuit->dim * circuit->dim * sizeof(f64));
    memset(circuit->b, 0, circuit->dim * sizeof(f64));

    // setup matrix
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        error_t err = DC_STAMPS[c->type](circuit->dim, circuit->A, circuit->b, c);

        if (err != OK) return err;
    }

    // solve
    error_t err = lu_solve(circuit->A, circuit->dim, circuit->b);

    // copy voltage values back into the nodes and cleanup
    if (err != OK) return err;
    circuit->nodes[0].potential = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        circuit->nodes[i].potential = circuit->b[i - 1];
    }

    return OK;
}
