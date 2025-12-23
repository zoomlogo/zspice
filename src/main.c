#include <stdio.h>
#include <math.h>

#include "analysis.h"
#include "circuit.h"
#include "component/component.h"
#include "dc.h"
#include "types.h"

i32 main(void) {
    circuit_t *circuit = new_circuit();

    component_t v1 = { VOLTAGE_SOURCE, 1, 0, .V.max_voltage = 5 };
    component_t r1 = { RESISTOR, 1, 2, .R.resistance = 100, .R.conductance = NAN };
    component_t r2 = { RESISTOR, 3, 0, .R.resistance = 100, .R.conductance = NAN };
    component_t r3 = { RESISTOR, 1, 4, .R.resistance = 10, .R.conductance = NAN };
    component_t c1 = { CAPACITOR, 4, 3, .C.capacitance = 1e-6 };
    component_t l1 = { INDUCTOR, 2, 3, .L.inductance = 0.01 };

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &r2);
    c_add_connection(circuit, &r3);
    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &l1);
    c_add_connection(circuit, &c1);

    c_init_solver_matrix(circuit, DC);

    dc_solve_linear(circuit);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf\n", i, circuit->nodes[i].potential);

    del_circuit(circuit);
    return 0;
}
