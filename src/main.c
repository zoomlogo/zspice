#include <stdio.h>
#include <math.h>

#include "circuit.h"
#include "component/component.h"
#include "dc.h"
#include "lu.h"
#include "types.h"

i32 main(void) {
    circuit_t *circuit = new_circuit();

    component_t i1 = { CURRENT_SOURCE, 1, 0, .I.current = 0.01 };
    component_t r1 = { RESISTOR, 0, 1, .R.resistance = 1000, .R.conductance = NAN };

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &i1);

    c_init_solver_matrix(circuit);

    dc_solve_linear(circuit);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    return 0;
}
