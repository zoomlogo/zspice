#include "component/component.h"
#include "component/resistor.h"
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

    component_t comp = { RESISTOR, 0, 1, (struct resistor) { 100, 0.01 } };
    c_add_connection(circuit, &comp);
    ASSERT(circuit->component_count == 1);
    ASSERT(circuit->node_count == 2);

    component_t comp2 = { RESISTOR, 100, 1, (struct resistor) { 1000, 0.001 } };
    c_add_connection(circuit, &comp2);
    ASSERT(circuit->component_count == 2);
    ASSERT(circuit->component_capacity == 4);
    ASSERT(circuit->node_count == 101);

    del_circuit(circuit);
}

void test_circuit(void) {
    BEGIN_TEST();

    test_circuit_instance();
    test_adding_components();

    END_TEST();
}
