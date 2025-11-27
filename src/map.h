#pragma once
#include "types.h"
#include "component.h"

// TODO Proper Error Handling.
// Since this is a library, not an application.

typedef struct {
    usize N; // number of spots allocated for connected nodes
    usize n; // number of actual connections - 1
    usize *Counts; // number of allocated space for each component in each node
    usize *counts; // number of components connected to each node - 1

    // key: [component1, component2, ...]
    usize *key; // node ids are keys of the linear map
    component_t **components; // components associated with the connected node
} map_t;

map_t *new_map();
void m_init(map_t *map);
void del_map(map_t *map);

void m_insert(map_t *map, usize id, component_t *component);
void m_retrieve(map_t *map, usize id, component_t **r_components, usize *r_len);
