#include "component/component.h"
#include "test.h"
#include "test_def.h"

#include "circuit.h"

static void test_circuit_instance(void) {
    circuit_t *circuit = new_circuit();

    ASSERT(circuit != NULL);
    ASSERT(circuit->components != NULL);
    ASSERT(circuit->nodes != NULL);

    del_circuit(circuit);
}

static void test_adding_components(void) {
    circuit_t *circuit = new_circuit();

    component_t comp = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = 0.01 };
    c_add_connection(circuit, &comp);
    ASSERT(circuit->component_count == 1);
    ASSERT(circuit->node_count == 2);

    component_t comp2 = { RESISTOR, 100, 1, .R.resistance = 1000, .R.conductance = 0.001 };
    c_add_connection(circuit, &comp2);
    ASSERT(circuit->component_count == 2);
    ASSERT(circuit->component_capacity == 4);
    ASSERT(circuit->node_count == 101);

    del_circuit(circuit);
}

static void test_init_matrix(void) {
    circuit_t *circuit = new_circuit();

    component_t r1 = { RESISTOR, 0, 1, .R.resistance = 5000, .R.conductance = 1 / 5000. };
    component_t r2 = { RESISTOR, 1, 2, .R.resistance = 5000, .R.conductance = 1 / 5000. };
    component_t r3 = { RESISTOR, 2, 3, .R.resistance = 5000, .R.conductance = 1 / 5000. };
    component_t v1 = { VOLTAGE_SOURCE, 0, 1, .V.potential = 5 };
    component_t v2 = { VOLTAGE_SOURCE, 1, 3, .V.potential = 4 };
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &r2);
    c_add_connection(circuit, &r3);
    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &v2);
    ASSERT(circuit->component_count == 5);
    ASSERT(circuit->node_count == 4);

    c_init_solver_matrix(circuit);
    ASSERT(circuit->dim == 6);
    ASSERT(circuit->A != NULL);
    ASSERT(circuit->b != NULL);

    del_circuit(circuit);
}

void test_circuit(void) {
    BEGIN_TEST();

    test_circuit_instance();
    test_adding_components();

    END_TEST();
}
