#include <stdlib.h>

#include "map.h"
#include "component/component.h"
#include "types.h"

i32 m_init(map_t *map) {
    map->N = 2;
    map->n = 0;

    map->connections = (connections_t *) calloc(map->N, sizeof(connections_t));
    if (map->connections == NULL) goto err_0;

    map->key = (usize *) calloc(map->N, sizeof(usize));
    if (map->key == NULL) goto err_1;

    return 0;

err_1:
    free(map->connections);
err_0:
    return -1;
}

void del_map(map_t *map) {
    free(map->key);

    for (usize i = 0; i < map->N; i++)
        free(map->connections[i].components);
    free(map->connections);
}

i32 m_insert(map_t *map, usize id, component_t *component) {
    // search
    i64 ii = -1;
    for (usize i = 0; i < map->n; i++)
        if (map->key[i] == id) {
            ii = i;
            break;
        }

    if (ii != -1) {
        // insert component into where id was found
        connections_t *connection = &map->connections[ii];

        if (connection->count >= connection->capacity) {
            // increase size
            usize new_capacity = connection->capacity == 0 ? 2 : connection->capacity * 2;

            component_t *components = (component_t *) realloc(connection->components, new_capacity * sizeof(component_t));
            if (components == NULL) return -1;

            connection->components = components;
            connection->capacity = new_capacity;
        }

        connection->components[connection->count++] = *component;

        return 0;
    } else {
        usize *key;

        if (map->n >= map->N) {
            usize new_N = 2 * map->N;
            key = (usize *) realloc(map->key, new_N * sizeof(usize));
            if (key == NULL) goto err_0;

            connections_t *connections = (connections_t *) realloc(map->connections, new_N * sizeof(connections_t));
            if (connections == NULL) goto err_1;

            for (usize i = map->n; i < new_N; i++) {
                key[i] = 0;
                connections[i].capacity = 0;
                connections[i].count = 0;
                connections[i].components = NULL;
            }

            map->connections = connections;
            map->key = key;
            map->N = new_N;
        }

        // add new component
        map->key[map->n] = id;
        ii = map->n++;
        connections_t *connection = &map->connections[ii];

        const usize new_capacity = 2;

        component_t *components = (component_t *) realloc(connection->components, new_capacity * sizeof(component_t));
        if (components == NULL) return -1;

        connection->components = components;
        connection->capacity = new_capacity;

        connection->components[connection->count++] = *component;

        return 0;

err_1:
        free(key);
err_0:
        return -1;
    }
}

i32 m_retrieve(map_t *map, usize id, connections_t *r_connections) {
    // search
    i64 ii = -1;
    for (usize i = 0; i < map->N; i++)
        if (map->key[i] == id) ii = i;

    if (ii == -1) return -1;

    *r_connections = map->connections[ii];

    return 0;
}
