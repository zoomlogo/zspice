#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "component.h"
#include "types.h"

/* Initializes a map object.
 * Allocates space for 2 connections each with a single component.
 */
void m_init(map_t *map) {
    // allocate space for atleast min 1 component each connected to 2 nodes.
    map->N = 2;
    map->n = 0;
    map->components = (component_t **) malloc(map->N * sizeof(component_t *));
    map->Counts = (usize *) malloc(map->N * sizeof(usize));
    map->counts = (usize *) malloc(map->N * sizeof(usize));

    for (usize i = 0; i < map->N; i++) {
        map->Counts[i] = 1;
        map->counts[i] = 0;
        map->components[i] = (component_t *) malloc(map->Counts[i] * sizeof(component_t));
    }
}

/* Must be called to delete a map_t object otherwise
 * memory can leak.
 */
void del_map(map_t *map) {
    for (usize i = 0; i < map->N; i++)
        free(map->components[i]);
    free(map->Counts);
    free(map->counts);
    free(map->key);
    free(map);
}

/* Insert a node along with a component into a map.
 * The component gets copied into the map.
 */
void m_insert(map_t *map, usize id, component_t *component) {
    // search
    i64 ii = -1;
    for (usize i = 0; i < map->N; i++)
        if (map->key[i] == id) ii = i;

    if (ii != -1) {
        // insert component into where id was found
        memcpy(&map->components[ii][map->counts[ii]], component, sizeof(component_t));
        map->counts[ii]++;

        if (map->counts[ii] >= map->Counts[ii]) {
            // increase size
            map->Counts[ii] *= 2;
            map->components[ii] = (component_t *) realloc(map->components[ii], map->Counts[ii] * sizeof(component_t));
        }
    } else {
        // add new component
        map->key[map->n] = id;
        map->n++;

        if (map->n >= map->N) {
            // increase size
            map->N *= 2;
            map->key = (usize *) realloc(map->key, map->N * sizeof(usize));
            map->components = (component_t **) realloc(map->components, map->N * sizeof(component_t *));
            map->Counts = (usize *) realloc(map->Counts, map->N * sizeof(usize));
            map->counts = (usize *) realloc(map->counts, map->N * sizeof(usize));

            // initialize the arrays
            for (usize i = map->n; i < map->N; i++) {
                map->Counts[i] = 1;
                map->counts[i] = 0;
                map->components[i] = (component_t *) malloc(map->Counts[i] * sizeof(component_t));
            }
        }
    }
}

/* Get all the components connected with node id by
 * copying the length into r_len and copying the pointer
 * to the components to r_components.
 */
void m_retrieve(map_t *map, usize id, component_t **r_components, usize *r_len) {
    // search
    i64 ii = -1;
    for (usize i = 0; i < map->N; i++)
        if (map->key[i] == id) ii = i;

    *r_components = map->components[ii];
    *r_len = map->counts[ii];
}
