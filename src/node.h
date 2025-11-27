#pragma once
#include "map.h"
#include "types.h"

typedef struct {
    map_t node_map;

    f64 potential;
    bool is_grounded;
} node_t;

node_t *new_node();
node_t *new_grounded_node();
void del_node(node_t *node);

