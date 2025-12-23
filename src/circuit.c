#include <stdlib.h>

#include "analysis.h"
#include "circuit.h"
#include "component/component.h"
#include "error.h"
#include "node.h"
#include "types.h"

circuit_t *new_circuit(void) {
    circuit_t *circuit = (circuit_t *) malloc(sizeof(circuit_t));
    if (circuit == NULL) goto err_0;

    circuit->node_count = 2;
    circuit->nodes = (node_t *) calloc(circuit->node_count, sizeof(node_t));
    if (circuit->nodes == NULL) goto err_1;

    circuit->component_capacity = 2;
    circuit->component_count = 0;
    circuit->components = (component_t *) calloc(circuit->component_capacity, sizeof(component_t));
    if (circuit->components == NULL) goto err_2;

    circuit->A = NULL;
    circuit->b = NULL;
    circuit->dim = 0;

    return circuit;

err_2:
    free(circuit->nodes);
err_1:
    free(circuit);
err_0:
    return NULL;
}

void del_circuit(circuit_t *circuit) {
    if (circuit == NULL) return;

    if (circuit->A != NULL) free(circuit->A);
    if (circuit->b != NULL) free(circuit->b);

    free(circuit->nodes);
    free(circuit->components);
    free(circuit);
}

error_t c_add_connection(circuit_t *circuit, const component_t *component) {
    // add connection by first verifying if the current nodes memory
    // block has enough slots to even solve for. if it doesnt then reallocate the block.
    usize higher_id = (component->id0 > component->id1) ? component->id0 : component->id1;
    if (higher_id >= circuit->node_count) {
        usize new_ncount = higher_id + 1;
        node_t *new_nodes = (node_t *) realloc(circuit->nodes, new_ncount * sizeof(node_t));
        if (new_nodes == NULL) return ERR_MEM_ALLOC;

        circuit->nodes = new_nodes;
        circuit->node_count = new_ncount;
    }

    circuit->components[circuit->component_count++] = *component;

    // check for overflow, and resize
    if (circuit->component_count >= circuit->component_capacity) {
        usize new_ccap = 2 * circuit->component_capacity;
        component_t *new_components = (component_t *) realloc(circuit->components, new_ccap * sizeof(component_t));
        if (new_components == NULL) return ERR_MEM_ALLOC;

        circuit->components = new_components;
        circuit->component_capacity = new_ccap;
    }

    return OK;
}

error_t c_init_solver_matrix(circuit_t *circuit, analysis_t analysis) {
    // certain components require the current flowing through a given branch
    // so that introduces an unknown
    // example: voltage sources / inductors
    usize unknwns = 0;
    for (usize i = 0; i < circuit->component_count; i++) {
        enum ctype type = circuit->components[i].type;
        if (type == VOLTAGE_SOURCE || type == INDUCTOR)
            circuit->components[i].solver_id = circuit->node_count - 1 + unknwns++;
    }

    // net dimension is node_count + unknwns minus one as node id
    // zero is always considered ground
    circuit->dim = circuit->node_count + unknwns - 1;

    switch (analysis) {
    case AC:
        circuit->A = calloc(circuit->dim * circuit->dim, sizeof(c64));
        if (circuit->A == NULL) goto err_0;

        circuit->b = calloc(circuit->dim, sizeof(c64));
        if (circuit->b == NULL) goto err_1;
    break;
    default:
        circuit->A = calloc(circuit->dim * circuit->dim, sizeof(f64));
        if (circuit->A == NULL) goto err_0;

        circuit->b = calloc(circuit->dim, sizeof(f64));
        if (circuit->b == NULL) goto err_1;
    }

    return OK;

err_1:
    free(circuit->A);
err_0:
    return ERR_MEM_ALLOC;
}
