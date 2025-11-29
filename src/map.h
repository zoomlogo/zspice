#pragma once
#include "types.h"
#include "component/component.h"

typedef struct {
    usize capacity;
    usize count;
    component_t *components;
} connections_t;

typedef struct {
    usize N;
    usize n;

    usize *key;
    connections_t *connections;
} map_t;

i32 m_init(map_t *map);
void del_map(map_t *map);

i32 m_insert(map_t *map, usize id, component_t *component);
i32 m_retrieve(map_t *map, usize id, connections_t *r_connections);
