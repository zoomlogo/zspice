#include <stdio.h>
#include <math.h>

#include "circuit.h"
#include "component/component.h"
#include "dc.h"
#include "lu.h"
#include "types.h"

// TODO testing for component/*.c

i32 main(void) {
    // make a very simple circuit for testing
    circuit_t *circuit = new_circuit();

    // 0----[- +]----1
    // |      v1     |
    // r4           r1
    // |             |
    // 4------r5-----2
    // |             |
    // r3           r2
    // |      v2     |
    // 5----[- +]----3
    component_t v1 = { VOLTAGE_SOURCE, 1, 0, .V.potential = 5 };
    component_t v2 = { VOLTAGE_SOURCE, 3, 5, .V.potential = 5 };
    component_t r1 = { RESISTOR, 1, 2, .R.resistance = 1000, .R.conductance = NAN };
    component_t r2 = { RESISTOR, 2, 3, .R.resistance = 680, .R.conductance = NAN };
    component_t r3 = { RESISTOR, 5, 4, .R.resistance = 3900, .R.conductance = NAN };
    component_t r4 = { RESISTOR, 4, 0, .R.resistance = 1800, .R.conductance = NAN };
    component_t r5 = { RESISTOR, 4, 2, .R.resistance = 680, .R.conductance = NAN };

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &r2);
    c_add_connection(circuit, &r3);
    c_add_connection(circuit, &r4);
    c_add_connection(circuit, &r5);

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &v2);

    c_init_solver_matrix(circuit);

    dc_solve(circuit);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    return 0;
}
