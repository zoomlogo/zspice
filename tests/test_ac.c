#include <math.h>
#include <stdio.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "component/component.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_circuit_ac(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit(); if (circuit == NULL) return;

    component_t v1 = { VOLTAGE_SOURCE, 1, 0, .V.max_voltage = 5, .V.frequency = NAN };
    component_t r1 = { RESISTOR, 1, 2, .R.resistance = 100, .R.conductance = NAN };
    component_t r2 = { RESISTOR, 3, 0, .R.resistance = 100, .R.conductance = NAN };
    component_t r3 = { RESISTOR, 1, 4, .R.resistance = 10, .R.conductance = NAN };
    component_t c1 = { CAPACITOR, 4, 3, .C.capacitance = 1e-6 };
    component_t l1 = { INDUCTOR, 2, 3, .L.inductance = 0.01 };

    ASSERT_OKC(c_add_connection(circuit, &r1));
    ASSERT_OKC(c_add_connection(circuit, &r2));
    ASSERT_OKC(c_add_connection(circuit, &r3));
    ASSERT_OKC(c_add_connection(circuit, &v1));
    ASSERT_OKC(c_add_connection(circuit, &l1));
    ASSERT_OKC(c_add_connection(circuit, &c1));

    ASSERT_OKC(c_calculate_dim(circuit));
    ASSERT_OKC(b_init(circuit->dim, true, &buf));
    ASSERT_OKC(e_set_frequency(&circuit->default_env, 40));

    ASSERT_OKC(ac_solve(circuit, &buf, NULL));
    // mags
    ASSERTC(circuit->nodes[0].zpotential, 0);
    ASSERTC(circuit->nodes[1].zpotential, 5);
    ASSERTC(circuit->nodes[2].zpotential, 2.500869 + J * 0.062830);
    ASSERTC(circuit->nodes[3].zpotential, 2.499290 + J * 0.000020);
    ASSERTC(circuit->nodes[4].zpotential, 4.999984 + J * -0.006285);

err:
    del_circuit(circuit);
    b_free(&buf);
}

void test_ac(void) {
    BEGIN_TEST();

    test_circuit_ac();

    END_TEST();
}
