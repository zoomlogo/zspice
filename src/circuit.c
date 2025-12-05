#include <stdlib.h>

#include "circuit.h"
#include "component/component.h"
#include "error.h"
#include "map.h"
#include "node.h"
#include "types.h"

circuit_t *new_circuit(usize N) {
    if (N < 2) goto err_0; // minimum 2 nodes for a circuit

    circuit_t *circuit = (circuit_t *) malloc(sizeof(circuit_t));
    if (circuit == NULL) goto err_0;

    circuit->nodes = (node_t *) calloc(N, sizeof(node_t));
    if (circuit->nodes == NULL) goto err_1;

    circuit->_n = 0;
    circuit->N = N;

    circuit->A = circuit->b = NULL;

    return circuit;

err_1:
    free(circuit);
err_0:
    return NULL;
}

error_t c_init_solver_matrix(circuit_t *circuit) {
    if (circuit == NULL) return ERR_INVALID_ARG;

    // Assign solver ids
    usize id_gen = 0;
    for (usize i = 0; i < circuit->N; i++) {
        node_t *node = &circuit->nodes[i];
        if (node->is_grounded) {
            node->solver_id = -1;
            continue;
        }
        node->solver_id = id_gen++;
    }

    // Count voltage sources.
    usize v_src = 0;
    for (usize i = 0; i < circuit->N; i++) {
        node_t *node = &circuit->nodes[i];
        for (usize j = 0; j < node->node_map.n; j++) {
            connections_t *conn = &node->node_map.connections[j];
            for (usize k = 0; k < conn->count; k++) {
                if (conn->components[k].type == VOLTAGE_SOURCE) v_src++;
            }
        }
    }
    // the voltage sources are counted twice with this method.
    // so we need to divide by 2

    // allocate matrix memory
    circuit->dim = id_gen + v_src / 2;
    circuit->A = (f64 *) calloc(circuit->dim * circuit->dim, sizeof(f64));
    if (circuit->A == NULL) goto err_0;

    circuit->b = (f64 *) calloc(circuit->dim, sizeof(f64));
    if (circuit->b == NULL) goto err_1;

    return OK;

err_1:
    free(circuit->A);
err_0:
    return ERR_MEM_ALLOC;
}

void del_circuit(circuit_t *circuit) {
    if (circuit == NULL) return;

    for (usize i = 0; i < circuit->_n; i++) {
        free(circuit->nodes[i].node_map.connections->components);
        free(circuit->nodes[i].node_map.connections);
        free(circuit->nodes[i].node_map.key);
    }

    free(circuit->nodes);

    free(circuit->A);
    free(circuit->b);

    free(circuit);
}

error_t c_add_node(circuit_t *circuit, node_t *node) {
    if (circuit == NULL || node == NULL) return ERR_INVALID_ARG;

    if (circuit->_n >= circuit->N) return ERR_NODE_OVERFLOW;

    if (circuit->_n == 0) { // ground this node, so we dont solve for this later
        node->is_grounded = true;
    }

    circuit->nodes[circuit->_n] = *node;
    circuit->nodes[circuit->_n].id = circuit->_n;

    circuit->_n++;

    return OK;
}
