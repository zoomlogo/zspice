#include "core/circuit.h"
#include "component/component.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(circuit_init)
{
	circuit_t *circuit = new_circuit();

	TEST_EXPECT(circuit != NULL);
	TEST_EXPECT(circuit->components != NULL);
	TEST_EXPECT(circuit->nodes != NULL);

	del_circuit(circuit);
}

TEST_DEFINE(adding_components)
{
	circuit_t *circuit = new_circuit();
	if (circuit == NULL)
		return;

	component_t comp = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = 0.01 };
	TEST_EXPECT_DEFER(c_add_connection(circuit, &comp) == OK);
	TEST_EXPECT(circuit->component_count == 1);
	TEST_EXPECT(circuit->node_count == 2);

	component_t comp2 = { RESISTOR, 100, 1, .R.resistance = 1000, .R.conductance = 0.001 };
	TEST_EXPECT_DEFER(c_add_connection(circuit, &comp2) == OK);
	TEST_EXPECT(circuit->component_count == 2);
	TEST_EXPECT(circuit->component_capacity == 4);
	TEST_EXPECT(circuit->node_count == 101);

defer:
	del_circuit(circuit);
}

TEST_DEFINE(dim_compute)
{
	circuit_t *circuit = new_circuit();
	if (circuit == NULL)
		return;

	component_t r1 = { RESISTOR, 0, 1 };
	component_t r2 = { RESISTOR, 1, 2 };
	component_t r3 = { RESISTOR, 1, 3 };
	component_t v1 = { VOLTAGE_SOURCE, 1, 0 };
	component_t v2 = { VOLTAGE_SOURCE, 3, 0 };
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r2) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &r3) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &v1) == OK);
	TEST_EXPECT_DEFER(c_add_connection(circuit, &v2) == OK);
	TEST_EXPECT(circuit->component_count == 5);
	TEST_EXPECT(circuit->node_count == 4);

	TEST_EXPECT_DEFER(c_calculate_dim(circuit) == OK);
	TEST_EXPECT(circuit->dim == 5);

defer:
	del_circuit(circuit);
}

void test_circuit(void)
{
	TEST_BEGIN();

	TEST_RUN(circuit_init);
	TEST_RUN(adding_components);
	TEST_RUN(dim_compute);

	TEST_END();
}
