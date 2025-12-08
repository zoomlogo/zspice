#include <stdio.h>
#include <stdlib.h>

#include "circuit.h"
#include "component/component.h"
#include "dc.h"
#include "lu.h"
#include "types.h"

i32 main(void) {
    // make a very simple circuit for testing
    circuit_t *circuit = new_circuit();

    component_t r = { RESISTOR, 0, 1, .R.resistance = 5000, .R.conductance = 1 / 5000. };
    component_t v = { VOLTAGE_SOURCE, 1, 0, .V.potential = 5 };
    c_add_connection(circuit, &r);
    c_add_connection(circuit, &v);

    c_init_solver_matrix(circuit);

    dc_solve(circuit);
    printf("%lf %lf\n", circuit->nodes[0].potential, circuit->nodes[1].potential);

    del_circuit(circuit);
    return 0;
}
