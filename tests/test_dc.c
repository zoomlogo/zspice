#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "component/component.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(simple_circuit)
{
	// make a simple circuit for testing
	sbuf_t buf;
	circuit_t *circuit = new_circuit();
	if (circuit == NULL)
		return;

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

	TEST_EXPECT_DEFER(c_add_connection(circuit, &r1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r2) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r3) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r4) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r5) == OK);

	TEST_EXPECT_DEFER(c_add_connection(circuit, &v1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &v2) == OK);

	error_e err = dc_solve_linear(circuit, &buf, NULL);
	TEST_EXPECT(err == ERR_NOT_INIT);

	TEST_EXPECT_DEFER(c_calculate_dim(circuit) == OK);
	TEST_EXPECT_DEFER(b_init(circuit->dim, false, &buf) == OK);

	TEST_EXPECT_DEFER(dc_solve_linear(circuit, &buf, NULL) == OK);

	TEST_EXPECT_FLOAT(circuit->nodes[0].potential, 0);
	TEST_EXPECT_FLOAT(circuit->nodes[1].potential, 5);
	TEST_EXPECT_FLOAT(circuit->nodes[2].potential, 3.71654);
	TEST_EXPECT_FLOAT(circuit->nodes[3].potential, 4.250101);
	TEST_EXPECT_FLOAT(circuit->nodes[4].potential, 2.310227);
	TEST_EXPECT_FLOAT(circuit->nodes[5].potential, -0.749899);

defer:
	del_circuit(circuit);
	b_free(&buf);
}

void test_dc(void)
{
	TEST_BEGIN();

	TEST_RUN(simple_circuit);

	TEST_END();
}
