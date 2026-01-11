#include <math.h>
#include <stdio.h>
#include <string.h>

#include "component/component.h"
#include "core/circuit.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "io/csv.h"
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
    for (usize i = 1; i < circuit->node_count; i++) {
        circuit->nodes[i].zpotential = zb[i - 1];
    }

    return OK;
}

error_e ac_sweep(circuit_t *circuit, ac_sweep_params_t *params, env_t *env) {
    // open file
    csv_t *csv = csv_open(params->filename);
    if (csv == NULL) return ERR_IO;

    csv_add_header(csv, "freq");
    for (usize i = 0; i < params->n; i++) {
        char buf[16];
        sprintf(buf, "Mag(V(%lu))", params->node_ids[i]);
        csv_add_header(csv, buf);

        sprintf(buf, "Ph(V(%lu))", params->node_ids[i]);
        csv_add_header(csv, buf);
    }
    csv_write_header(csv);

    // initialize
    if (env == NULL) env = &circuit->default_env;
    sbuf_t buffer; b_init(circuit->dim, true, &buffer);

    // important: find operation point
    // operating params stored back into the components themselves
    dc_solve_non_linear(circuit, &buffer, env);

    // sweep types
    f64 current_freq = params->start_frequency;

    f64 net_steps;
    f64 step_size;
    f64 multiplier;

    switch (params->sweep_type) {
    case SWEEP_LINEAR:
        net_steps = params->steps;
        multiplier = 1;
        step_size = (params->stop_frequency - params->start_frequency) / (params->steps - 1);
        break;
    case SWEEP_DECADE:
        net_steps = params->steps * log10(params->stop_frequency / params->start_frequency) + 1;
        multiplier = pow(10., 1. / (double) params->steps);
        step_size = 0;
        break;
    case SWEEP_OCTAVE:
        net_steps = params->steps * log2(params->stop_frequency / params->start_frequency) + 1;
        multiplier = pow(2., 1. / (double) params->steps);
        step_size = 0;
        break;
    }

    // sweep laazik
    for (usize i = 0; i < net_steps; i++) {
        e_set_frequency(env, current_freq);
        ac_solve(circuit, &buffer, NULL);

        // write results
        csv_write_data(csv, current_freq);
        for (usize k = 0; k < params->n; k++) {
            usize id = params->node_ids[k];
            usize ref_id = params->ref_node_id;

            c64 ref_V;

            if (ref_id == 0) ref_V = 1;
            else ref_V = circuit->nodes[ref_id].zpotential;

            // compute gain w.r.t. reference
            c64 V = circuit->nodes[id].zpotential / ref_V;

            csv_write_data(csv, 20 * zlog10(cabs(V)));
            csv_write_data(csv, carg(V) * 180 / M_PI);
        }

        // next frequency
        current_freq *= multiplier;
        current_freq += step_size;
        // clamp
        if (i == net_steps - 2) current_freq = params->stop_frequency;
    }

    // cleanup
    csv_close(csv);
    b_free(&buffer);
    return OK;
}
