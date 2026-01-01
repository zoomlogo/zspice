#pragma once
#include "component/component.h"
#include "util/error.h"

#include "node.h"
#include "types.h"

// Stores the different types of analysis.
typedef enum {
    AC,
    DC,
    TRANISENT
} analysis_e;

// Stores information about the environment of the circuit.
typedef struct {
    // Temperature
    f64 temperature; // in Kelvin.
    f64 V_T; // thermal voltage

    // AC analysis
    f64 w; // angular frequency
    f64 f; // frequency
} env_t;


// Stores circuits as a collection of nodes and components.
typedef struct {
    usize node_count;
    node_t *nodes;

    usize component_capacity;
    usize component_count;
    component_t *components;

    usize dim; // the number of unknowns in the circuit
} circuit_t;

circuit_t *new_circuit(void);
void del_circuit(circuit_t *circuit);

error_e c_add_connection(circuit_t *circuit, const component_t *component);
error_e c_calculate_dim(circuit_t *circuit);

// Stores the matrix which is to be solved.
typedef struct {
    usize dim;
    void *A; // dim×dim matrix
    void *b; // dim vector
} sbuf_t;

error_e b_init(circuit_t *circuit, analysis_e analysis, sbuf_t *solver_buffer);
