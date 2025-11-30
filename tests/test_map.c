#include <stdlib.h>

#include "component/component.h"
#include "map.h"
#include "types.h"

#include "test.h"
#include "test_def.h"

void test_map(void) {
    BEGIN_TEST();

    i32 err;
    map_t map;
    m_init(&map);

    component_t *c1 = new_component(RESISTOR);
    component_t *c2 = new_component(RESISTOR);
    component_t *c3 = new_component(VOLTAGE_SOURCE);

    // insertion
    m_insert(&map, 0, c1);
    m_insert(&map, 1, c2);
    m_insert(&map, 2, c1);
    m_insert(&map, 2, c3);
    ASSERT(map.N == 4);

    m_insert(&map, 4, c1);
    m_insert(&map, 4, c2);
    m_insert(&map, 4, c3);
    ASSERT(map.n == 4);
    ASSERT(map.N == 4);

    m_insert(&map, 9, c2);
    ASSERT(map.n == 5);
    ASSERT(map.N == 8);

    // retrieval
    connections_t conn;
    err = m_retrieve(&map, 10, &conn);
    ASSERT(err == -1);
    err = m_retrieve(&map, 4, &conn);
    ASSERT(err == 0);
    ASSERT(conn.count == 3);
    ASSERT(conn.components[0].type == RESISTOR);
    ASSERT(conn.components[2].type == VOLTAGE_SOURCE);

    // cleanup
    del_map(&map);
    free(c1);
    free(c2);
    free(c3);

    END_TEST();
}
