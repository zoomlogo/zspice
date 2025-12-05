#pragma once
#include "error.h"
#include "node.h"

typedef struct {
    usize _n;

    usize N;
    node_t *nodes;

    // solver matrix
    f64 *A;
    f64 *b;
    usize dim;
} circuit_t;

circuit_t *new_circuit(usize N);
error_t c_init_solver_matrix(circuit_t *circuit);
void del_circuit(circuit_t *circuit);

error_t c_add_node(circuit_t *circuit, node_t *node);
