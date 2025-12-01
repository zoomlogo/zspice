#include <stdlib.h>

#include "map.h"
#include "node.h"

node_t *new_node() {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    if (node == NULL) goto err_0;

    i32 err = m_init(&node->node_map);
    if (err) goto err_1;

    node->is_grounded = false;
    node->potential = 0;
    node->id = 0;
    node->solver_id = 0;

    return node;

err_1:
    free(node);
err_0:
    return NULL;
}

void del_node(node_t *node) {
    if (node == NULL) return;

    del_map(&node->node_map);
    free(node);
}
