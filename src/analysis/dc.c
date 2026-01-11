#include <string.h>

#include "component/component.h"
#include "core/circuit.h"
#include "core/environment.h"
#include "io/csv.h"
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

error_e dc_sweep(circuit_t *circuit, dc_sweep_params_t *params, env_t *env) {
    // open file
    csv_t *csv = csv_open(params->filename);
    if (csv == NULL) return ERR_IO;

    csv_add_header(csv, "voltage");
    for (usize i = 0; i < params->n; i++) {
        char buf[16];
        sprintf(buf, "V(%lu)", params->node_ids[i]);
        csv_add_header(csv, buf);
    }
    csv_write_header(csv);

    // initialize
    if (env == NULL) env = &circuit->default_env;
    sbuf_t buffer; b_init(circuit->dim, true, &buffer);

    // sweep types
    f64 current_voltage = params->start_voltage;

    f64 net_steps;
    f64 step_size;
    f64 multiplier;

    switch (params->sweep_type) {
    case SWEEP_LINEAR:
        net_steps = params->steps;
        multiplier = 1;
        step_size = (params->stop_voltage - params->start_voltage) / (params->steps - 1);
        break;
    case SWEEP_DECADE:
        net_steps = params->steps * log10(params->stop_voltage / params->start_voltage) + 1;
        multiplier = pow(10., 1. / (double) params->steps);
        step_size = 0;
        break;
    case SWEEP_OCTAVE:
        net_steps = params->steps * log2(params->stop_voltage / params->start_voltage) + 1;
        multiplier = pow(2., 1. / (double) params->steps);
        step_size = 0;
        break;
    }

    // sweep laazik
    for (usize i = 0; i < net_steps; i++) {
        circuit->components[params->sweeped_component_id].V.dc_offset = current_voltage;
        dc_solve_non_linear(circuit, &buffer, NULL);

        // write results
        csv_write_data(csv, current_voltage);
        for (usize k = 0; k < params->n; k++) {
            usize id = params->node_ids[k];
            csv_write_data(csv, circuit->nodes[id].potential);
        }

        // next frequency
        current_voltage *= multiplier;
        current_voltage += step_size;
        // clamp
        if (i == net_steps - 2) current_voltage = params->stop_voltage;
    }

    // cleanup
    csv_close(csv);
    b_free(&buffer);
    return OK;
}
