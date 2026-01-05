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
    const f64 f = 40;

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t ss = { VOLTAGE_SOURCE, 0, 2 }; c_defaults(&ss); ss.V.max_voltage = 0;

    component_t cccs1 = { CCCS, 3, 0, 5 }; c_defaults(&cccs1);
    component_t r2 = { RESISTOR, 3, 0 }; c_defaults(&r2);

    component_t ccvs1 = { CCVS, 4, 0, 5 }; c_defaults(&ccvs1);
    component_t r3 = { RESISTOR, 4, 0 }; c_defaults(&r3);

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &ss);

    c_add_connection(circuit, &cccs1);
    c_add_connection(circuit, &r2);

    c_add_connection(circuit, &ccvs1);
    c_add_connection(circuit, &r3);

    c_calculate_dim(circuit);
    b_init(circuit->dim, false, &buf);

    dc_solve_linear(circuit, &buf, NULL);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
