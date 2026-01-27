#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"

#include "types.h"
#include "util/error.h"
#include "util/log.h"

i32 main(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit();

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);

    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t r2 = { RESISTOR, 2, 0 }; c_defaults(&r2);

    component_t r3 = { RESISTOR, 1, 3 }; c_defaults(&r3);
    component_t r4 = { RESISTOR, 4, 0 }; c_defaults(&r4);

    component_t q1 = { BJT, 2, 4, 3 }; c_defaults(&q1);
    q1.Q.Va = 15; q1.Q.Cj0c = 1e-4; q1.Q.Cj0e = 1e-3;
    q1.Q.tau_f = 0.1; q1.Q.tau_r = 0.01;

    c_add_connection(circuit, &v1);

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &r2);
    c_add_connection(circuit, &r3);
    c_add_connection(circuit, &r4);

    c_add_connection(circuit, &q1);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    error_e err = dc_solve_non_linear(circuit, &buf, NULL);
    if (err != OK) log_error("%s", err_str(err));
    for (usize i = 0; i < circuit->node_count; i++) {
        log_info("%zu| %lf", i, circuit->nodes[i].potential);
    }

    log_info("dumping transistor parameters");
    struct bjt Q = circuit->components[5].Q;
    log_info("g_pi(%lf) g_mu(%lf) g_mf(%lf) g_mr(%lf) g_o(%lf)", Q.g_pi, Q.g_mu, Q.g_mf, Q.g_mr, Q.g_o);
    log_info("c_be(%lf) c_bc(%lf)", Q.c_be, Q.c_bc);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
