#include <stdlib.h>

#include "map.h"
#include "node.h"

node_t *new_node() {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    m_init(&node->node_map);
    node->is_grounded = false;
    return node;
}

node_t *new_grounded_node() {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    m_init(&node->node_map);
    node->is_grounded = true;
    node->potential = 0;
    return node;
}

void del_node(node_t *node) {
    del_map(&node->node_map);
    free(node);
}
