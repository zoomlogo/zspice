#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_simple_circuit(void) {
    // make a simple circuit for testing
    sbuf_t buf;
    circuit_t *circuit = new_circuit(); if (circuit == NULL) return;
    error_e err;

    // 0----[- +]----1
    // |      v1     |
    // r4           r1
    // |             |
    // 4------r5-----2
    // |             |
    // r3           r2
    // |      v2     |
    // 5----[- +]----3
    component_t v1 = { VOLTAGE_SOURCE, 1, 0, .V.dc_offset = 5 };
    component_t v2 = { VOLTAGE_SOURCE, 3, 5, .V.dc_offset = 5 };
    component_t r1 = { RESISTOR, 1, 2, .R.resistance = 1000, .R.conductance = NAN };
    component_t r2 = { RESISTOR, 2, 3, .R.resistance = 680, .R.conductance = NAN };
    component_t r3 = { RESISTOR, 5, 4, .R.resistance = 3900, .R.conductance = NAN };
    component_t r4 = { RESISTOR, 4, 0, .R.resistance = 1800, .R.conductance = NAN };
    component_t r5 = { RESISTOR, 4, 2, .R.resistance = 680, .R.conductance = NAN };

    err = c_add_connection(circuit, &r1); if (err != OK) goto err;
    err = c_add_connection(circuit, &r2); if (err != OK) goto err;
    err = c_add_connection(circuit, &r3); if (err != OK) goto err;
    err = c_add_connection(circuit, &r4); if (err != OK) goto err;
    err = c_add_connection(circuit, &r5); if (err != OK) goto err;

    err = c_add_connection(circuit, &v1); if (err != OK) goto err;
    err = c_add_connection(circuit, &v2); if (err != OK) goto err;

    err = dc_solve_linear(circuit, &buf, NULL);
    ASSERT(err == ERR_NOT_INIT);

    err = c_calculate_dim(circuit); if (err != OK) goto err;
    err = b_init(circuit->dim, false, &buf); if (err != OK) goto err;

    err = dc_solve_linear(circuit, &buf, NULL);
    ASSERT(err == OK);

    ASSERTF(circuit->nodes[0].potential, 0);
    ASSERTF(circuit->nodes[1].potential, 5);
    ASSERTF(circuit->nodes[2].potential, 3.71654);
    ASSERTF(circuit->nodes[3].potential, 4.250101);
    ASSERTF(circuit->nodes[4].potential, 2.310227);
    ASSERTF(circuit->nodes[5].potential, -0.749899);

err:
    del_circuit(circuit);
}

void test_dc(void) {
    BEGIN_TEST();

    test_simple_circuit();

    END_TEST();
}
