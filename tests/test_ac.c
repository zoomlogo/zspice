#include <math.h>
#include <stdio.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "component/component.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(circuit_ac)
{
	sbuf_t buf;
	circuit_t *circuit = new_circuit();
	if (circuit == NULL)
		return;

	component_t v1 = { VOLTAGE_SOURCE, 1, 0,.V.max_voltage =
		    5,.V.frequency = NAN };
	component_t r1 = { RESISTOR, 1, 2,.R.resistance = 100,.R.conductance =
		    NAN };
	component_t r2 = { RESISTOR, 3, 0,.R.resistance = 100,.R.conductance =
		    NAN };
	component_t r3 = { RESISTOR, 1, 4,.R.resistance = 10,.R.conductance =
		    NAN };
	component_t c1 = { CAPACITOR, 4, 3,.C.capacitance = 1e-6 };
	component_t l1 = { INDUCTOR, 2, 3,.L.inductance = 0.01 };

	TEST_EXPECT_DEFER(c_add_connection(circuit, &r1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r2) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r3) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &v1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &l1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &c1) == OK);

	TEST_EXPECT_DEFER(c_calculate_dim(circuit) == OK);
	TEST_EXPECT_DEFER(b_init(circuit->dim, true, &buf) == OK);
	TEST_EXPECT_DEFER(e_set_frequency(&circuit->default_env, 40) == OK);

	TEST_EXPECT_DEFER(ac_solve(circuit, &buf, NULL) == OK);
	// mags
	TEST_EXPECT_FLOAT(creal(circuit->nodes[0].zpotential), 0.0);
	TEST_EXPECT_FLOAT(cimag(circuit->nodes[0].zpotential), 0.0);
	TEST_EXPECT_FLOAT(creal(circuit->nodes[1].zpotential), 5.0);
	TEST_EXPECT_FLOAT(cimag(circuit->nodes[1].zpotential), 0.0);
	TEST_EXPECT_FLOAT(creal(circuit->nodes[2].zpotential), 2.500869);
	TEST_EXPECT_FLOAT(cimag(circuit->nodes[2].zpotential), 0.062830);
	TEST_EXPECT_FLOAT(creal(circuit->nodes[3].zpotential), 2.499290);
	TEST_EXPECT_FLOAT(cimag(circuit->nodes[3].zpotential), 0.000020);
	TEST_EXPECT_FLOAT(creal(circuit->nodes[4].zpotential), 4.999984);
	TEST_EXPECT_FLOAT(cimag(circuit->nodes[4].zpotential), -0.006285);

defer:
	del_circuit(circuit);
	b_free(&buf);
}

void test_ac(void)
{
	TEST_BEGIN();

	TEST_RUN(circuit_ac);

	TEST_END();
}
