#pragma once
#include "component/component.h"
#include "util/error.h"

#include "environment.h"
#include "node.h"
#include "types.h"

// Stores circuits as a collection of nodes and components.
typedef struct {
    usize node_count;
    node_t *nodes;

    usize component_capacity;
    usize component_count;
    component_t *components;

    usize dim; // the number of unknowns in the circuit
    env_t default_env;
} circuit_t;

circuit_t *new_circuit(void);
void del_circuit(circuit_t *circuit);

error_e c_add_connection(circuit_t *circuit, const component_t *component);
error_e c_calculate_dim(circuit_t *circuit);
