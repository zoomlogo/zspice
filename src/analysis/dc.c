#include <string.h>

#include "component/component.h"
#include "core/circuit.h"
#include "core/environment.h"
#include "util/lu.h"
#include "util/error.h"

#include "analysis.h"

error_e dc_solve_linear(circuit_t *circuit, sbuf_t *buffer, env_t *env) {
    if (circuit->dim == 0) return ERR_NOT_INIT;
    if (buffer->dim == 0) return ERR_NOT_INIT;
    if (buffer->dim != circuit->dim) return ERR_INVALID_ARG;

    if (env == NULL) env = &circuit->default_env;

    f64 *A = buffer->A;
    f64 *b = buffer->b;

    // reset memory
    memset(A, 0, buffer->dim * buffer->dim * sizeof(f64));
    memset(b, 0, buffer->dim * sizeof(f64));

    // setup matrix
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        error_e err = DC_STAMPS[c->type](buffer, c, env);

        if (err != OK) return err;
    }

    // solve
    error_e err = r_lu_solve(A, buffer->dim, b);
    if (err != OK) return err;

    // copy voltage values back into the nodes and cleanup
    circuit->nodes[0].potential = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        circuit->nodes[i].potential = b[i - 1];
    }

    return OK;
}

error_e dc_solve_non_linear(circuit_t *circuit, sbuf_t *buffer, env_t *env) {
    if (circuit->dim == 0) return ERR_NOT_INIT;
    if (buffer->dim == 0) return ERR_NOT_INIT;

    if (env == NULL) env = &circuit->default_env;

    f64 *A = (f64 *) buffer->A;
    f64 *b = (f64 *) buffer->b;

    for (int k = 0; k < MAX_ITERATIONS; k++) {
        // reset memory
        memset(A, 0, buffer->dim * buffer->dim * sizeof(f64));
        memset(b, 0, buffer->dim * sizeof(f64));

        // linearize
        dc_linearize(circuit, env);

        // setup matrix
        for (usize i = 0; i < circuit->component_count; i++) {
            component_t *c = &circuit->components[i];
            error_e err = DC_STAMPS[c->type](buffer, c, env);

            if (err != OK) return err;
        }

        // solve
        error_e err = r_lu_solve(A, buffer->dim, b);
        if (err != OK) return err;

        // update guesses
        err = dc_update_guesses(circuit, buffer);
        if (err != OK) return err;

        // check for convergence
        if (dc_check_convergence(circuit)) {
            break; // converged
        }
    }

    dc_linearize(circuit, env);

    // copy voltage values back into the nodes and cleanup
    circuit->nodes[0].potential = 0;
    for (usize i = 1; i < circuit->node_count; i++) {
        circuit->nodes[i].potential = b[i - 1];
    }

    return OK;
}
