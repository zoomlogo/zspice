#include "core/circuit.h"
#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"


static void test_circuit_instance(void) {
    circuit_t *circuit = new_circuit();

    ASSERT(circuit != NULL);
    ASSERT(circuit->components != NULL);
    ASSERT(circuit->nodes != NULL);

    del_circuit(circuit);
}

static void test_adding_components(void) {
    circuit_t *circuit = new_circuit(); if (circuit == NULL) return;

    component_t comp = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = 0.01 };
    ASSERT_OKC(c_add_connection(circuit, &comp));
    ASSERT(circuit->component_count == 1);
    ASSERT(circuit->node_count == 2);

    component_t comp2 = { RESISTOR, 100, 1, .R.resistance = 1000, .R.conductance = 0.001 };
    ASSERT_OKC(c_add_connection(circuit, &comp2));
    ASSERT(circuit->component_count == 2);
    ASSERT(circuit->component_capacity == 4);
    ASSERT(circuit->node_count == 101);

err:
    del_circuit(circuit);
}

static void test_dim_compute(void) {
    circuit_t *circuit = new_circuit(); if (circuit == NULL) return;

    component_t r1 = { RESISTOR, 0, 1 };
    component_t r2 = { RESISTOR, 1, 2 };
    component_t r3 = { RESISTOR, 1, 3 };
    component_t v1 = { VOLTAGE_SOURCE, 1, 0 };
    component_t v2 = { VOLTAGE_SOURCE, 3, 0 };
    ASSERT_OKC(c_add_connection(circuit, &r1));
    ASSERT_OKC(c_add_connection(circuit, &r2));
    ASSERT_OKC(c_add_connection(circuit, &r3));
    ASSERT_OKC(c_add_connection(circuit, &v1));
    ASSERT_OKC(c_add_connection(circuit, &v2));
    ASSERT(circuit->component_count == 5);
    ASSERT(circuit->node_count == 4);

    ASSERT_OKC(c_calculate_dim(circuit));
    ASSERT(circuit->dim == 5);

err:
    del_circuit(circuit);
}

void test_circuit(void) {
    BEGIN_TEST();

    test_circuit_instance();
    test_adding_components();
    test_dim_compute();

    END_TEST();
}
