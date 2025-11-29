#include <stdio.h>
#include <stdlib.h>

#include "circuit.h"
#include "component/component.h"
#include "lu.h"
#include "map.h"
#include "node.h"
#include "types.h"

i32 main(void) {
    node_t *a = new_node();
    node_t *b = new_node();

    component_t *resistor = new_component(RESISTOR);
    resistor->R.resistance = 1000.0;
    resistor->R.conductance = 1 / 1000.0;

    component_t *source = new_component(VOLTAGE_SOURCE);
    source->V.potential = 10.0;

    a->is_grounded = true;

    circuit_t *circuit = new_circuit(2);
    c_add_node(circuit, a);
    c_add_node(circuit, b);

    // resistor from a -> b and b -> a
    m_insert(&circuit->nodes[0].node_map, circuit->nodes[1].id, resistor);
    m_insert(&circuit->nodes[1].node_map, circuit->nodes[0].id, resistor);

    // insert voltage source from a -> b and b -> a
    m_insert(&circuit->nodes[0].node_map, circuit->nodes[1].id, source);
    source->V.potential = -10.0;
    m_insert(&circuit->nodes[1].node_map, circuit->nodes[0].id, source);


    // print circuit
    for (usize i = 0; i < circuit->N; i++) {
        printf("i(%lu)\n ", i);

        node_t *n = circuit->nodes + i;
        printf("node id(%lu):\n  ", n->id);

        for (usize j = 0; j < n->node_map.n; j++) {
            printf("|| %d: ", n->node_map.key[j]);
            printf("%lu", n->node_map.connections->count);
            printf(" ||\n");
        }
    }

    // free memory
    del_circuit(circuit);
    del_node(a);
    del_node(b);

    free(resistor);
    free(source);

    return 0;
}
