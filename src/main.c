#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"

#include "types.h"

i32 main(void) {
    sbuf_t buf;

    circuit_t *circuit = new_circuit();
    const f64 f = 40;

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 0 }; c_defaults(&r1);

    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &v1);

    c_calculate_dim(circuit);
    b_init(circuit, DC, &buf);

    dc_solve_linear(circuit, &buf, NULL);
    for (usize i = 0; i < circuit->node_count; i++)
        printf("%lu: %lf∠%lf\n", i, circuit->nodes[i].potential, circuit->nodes[i].phase);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
