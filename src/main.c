#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"
#include "io/csv.h"

#include "types.h"

i32 main(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit();

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t d1 = { DIODE, 2, 3 }; c_defaults(&d1);
    component_t d2 = { DIODE, 3, 0 }; c_defaults(&d2);

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &d1);
    c_add_connection(circuit, &d2);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    dc_solve_non_linear(circuit, &buf, NULL);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%zu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
