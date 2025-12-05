#include <stdlib.h>

#include "circuit.h"
#include "error.h"
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

    return circuit;

err_1:
    free(circuit);
err_0:
    return NULL;
}

void del_circuit(circuit_t *circuit) {
    if (circuit == NULL) return;

    for (usize i = 0; i < circuit->_n; i++) {
        free(circuit->nodes[i].node_map.connections->components);
        free(circuit->nodes[i].node_map.connections);
        free(circuit->nodes[i].node_map.key);
    }

    free(circuit->nodes);
    free(circuit);
}

error_t c_add_node(circuit_t *circuit, node_t *node) {
    if (circuit == NULL || node == NULL) return ERR_INVALID_ARG;

    if (circuit->_n >= circuit->N) return ERR_NODE_OVERFLOW;

    circuit->nodes[circuit->_n] = *node;
    circuit->nodes[circuit->_n].id = circuit->_n;

    circuit->_n++;

    return OK;
}
