#pragma once
#include "component/component.h"
#include "error.h"
#include "node.h"
#include "types.h"

typedef struct {
    usize node_count;
    node_t *nodes;  // stores potentials (for now)

    usize component_capacity;
    usize component_count;
    component_t *components;

    // solver matrix
    f64 *A;
    f64 *b;
    usize dim;
} circuit_t;

circuit_t *new_circuit(void);
void del_circuit(circuit_t *circuit);

error_t c_add_connection(circuit_t *circuit, const component_t *component);
error_t c_init_solver_matrix(circuit_t *circuit);
