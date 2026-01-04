#include <math.h>
#include <string.h>

#include "component/component.h"
#include "core/circuit.h"
#include "util/error.h"
#include "util/lu.h"

#include "analysis.h"
#include "types.h"

error_e ac_solve(circuit_t *circuit, sbuf_t *buffer, env_t *env) {
    // assuming circuit is linearized at this point...
    if (circuit->dim == 0) return ERR_NOT_INIT;
    if (buffer->dim == 0) return ERR_NOT_INIT;
    if (buffer->dim != circuit->dim) return ERR_INVALID_ARG;

    if (env == NULL) env = &circuit->default_env;

    c64 *zA = buffer->zA;
    c64 *zb = buffer->zb;

    // reset memory
    memset(zA, 0, buffer->dim * buffer->dim * sizeof(c64));
    memset(zb, 0, buffer->dim * sizeof(c64));

    // setup
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        error_e err = AC_STAMPS[c->type](buffer, c, env);

        if (err != OK) return err;
    }

    // solve
    error_e err = c_lu_solve(zA, buffer->dim, zb);

    // copy mag+phase
    if (err != OK) return err;
    circuit->nodes[0].potential = 0;
    circuit->nodes[0].phase = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        c64 V = zb[i - 1]; // AC phasor
        circuit->nodes[i].potential = cabs(V);
        circuit->nodes[i].phase = carg(V) * 180 / M_PI;
    }

    return OK;
}
