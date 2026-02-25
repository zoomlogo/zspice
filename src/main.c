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

    component_t qm = { MOSFET, 2, 3, 4, 4 }; c_defaults(&qm);

    c_add_connection(circuit, &v1);

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &r2);
    c_add_connection(circuit, &r3);
    c_add_connection(circuit, &r4);

    c_add_connection(circuit, &qm);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    error_e err = dc_solve_non_linear(circuit, &buf, NULL);
    if (err != OK) log_error("%s", err_str(err));

    for (usize i = 0; i < circuit->node_count; i++)
        log_info("i(%zu): V(%lf)", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
