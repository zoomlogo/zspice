#include <stdlib.h>

#include "component/component.h"
#include "map.h"
#include "types.h"

#include "test.h"

i32 main(void) {
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
    ASSERT(map.N == 3);

    // cleanup
    del_map(&map);
    free(c1);
    free(c2);
    free(c3);

    return 0;
}
