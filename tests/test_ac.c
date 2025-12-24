#include <math.h>

#include "circuit.h"
#include "component/component.h"
#include "error.h"
#include "ac.h"
#include "test.h"

static void test_circuit(void) {
    circuit_t *circuit = new_circuit(); if (circuit == NULL) return;
    error_e err;

    component_t v1 = { VOLTAGE_SOURCE, 1, 0, .V.max_voltage = 5, .V.frequency = NAN };
    component_t r1 = { RESISTOR, 1, 2, .R.resistance = 100, .R.conductance = NAN };
    component_t r2 = { RESISTOR, 3, 0, .R.resistance = 100, .R.conductance = NAN };
    component_t r3 = { RESISTOR, 1, 4, .R.resistance = 10, .R.conductance = NAN };
    component_t c1 = { CAPACITOR, 4, 3, .C.capacitance = 1e-6 };
    component_t l1 = { INDUCTOR, 2, 3, .L.inductance = 0.01 };

    err = c_add_connection(circuit, &r1); if (err != OK) goto err;
    err = c_add_connection(circuit, &r2); if (err != OK) goto err;
    err = c_add_connection(circuit, &r3); if (err != OK) goto err;
    err = c_add_connection(circuit, &v1); if (err != OK) goto err;
    err = c_add_connection(circuit, &l1); if (err != OK) goto err;
    err = c_add_connection(circuit, &c1); if (err != OK) goto err;

    err = c_init_solver_matrix(circuit, AC); if (err != OK) goto err;

    err = ac_solve(circuit, 2 * M_PI * 40);
    ASSERT(err == OK);
    // mags
    ASSERTF(circuit->nodes[0].potential, 0);
    ASSERTF(circuit->nodes[1].potential, 5);
    ASSERTF(circuit->nodes[2].potential, 2.500868218078);
    ASSERTF(circuit->nodes[3].potential, 2.501656730276);
    ASSERTF(circuit->nodes[4].potential, 4.999830306554);
    // phases
    ASSERTF(circuit->nodes[0].phase, 0);
    ASSERTF(circuit->nodes[1].phase, 0);
    ASSERTF(circuit->nodes[2].phase, -0.000000872222);
    ASSERTF(circuit->nodes[3].phase, 0.025109140882);
    ASSERTF(circuit->nodes[4].phase, -0.001256156358);

err:
    del_circuit(circuit);
}

void test_ac(void) {
    BEGIN_TEST();

    test_circuit();

    END_TEST();
}
