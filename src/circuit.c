#include <stdlib.h>

#include "circuit.h"
#include "node.h"
#include "types.h"

circuit_t *new_circuit(usize N) {
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
    // TODO free the components
    free(circuit->nodes);
    free(circuit);
}

i32 c_add_node(circuit_t *circuit, node_t *node) {
    if (circuit->_n >= circuit->N) return -1;

    circuit->nodes[circuit->_n] = *node;
    circuit->nodes[circuit->_n].id = circuit->_n;

    circuit->_n++;

    return 0;
}
