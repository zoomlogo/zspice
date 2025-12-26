#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "component/component.h"

#include "types.h"

i32 main(void) {
    circuit_t *circuit = new_circuit();
    const f64 f = 40;

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 0 }; c_defaults(&r1);

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &v1);

    c_init_solver_matrix(circuit, DC);

    dc_solve_linear(circuit);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf∠%lf\n", i, circuit->nodes[i].potential, circuit->nodes[i].phase);

    del_circuit(circuit);
    return 0;
}
