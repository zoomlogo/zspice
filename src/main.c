#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"

#include "types.h"

i32 main(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit();

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    // sense points in direction of current
    component_t sense = { VOLTAGE_SOURCE, 0, 2 }; c_defaults(&sense); sense.V.max_voltage = 0;

    component_t ccs = { CCCS, 3, 0, 4 }; c_defaults(&ccs);
    component_t r2 = { RESISTOR, 3, 0 }; c_defaults(&r2);

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &sense);

    c_add_connection(circuit, &ccs);
    c_add_connection(circuit, &r2);

    c_calculate_dim(circuit);
    b_init(circuit->dim, false, &buf);

    dc_solve_linear(circuit, &buf, NULL);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
