#pragma once
#include "map.h"
#include "types.h"

// TODO error on arg checking

typedef struct {
    map_t node_map;
    usize id;

    i64 solver_id;
    f64 potential;
    bool is_grounded;
} node_t;

node_t *new_node();
void del_node(node_t *node);

