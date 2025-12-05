#pragma once
#include "error.h"
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

error_t m_init(map_t *map);
void del_map(map_t *map);

error_t m_insert(map_t *map, usize id, component_t *component);
error_t m_retrieve(map_t *map, usize id, connections_t *r_connections);
