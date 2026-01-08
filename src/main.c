#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"
#include "io/csv.h"

#include "types.h"

i32 main(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit();
    e_set_frequency(&circuit->default_env, 40);

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t l1 = { INDUCTOR, 2, 0 }; c_defaults(&l1); l1.L.inductance = 1;

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &l1);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    ac_solve(circuit, &buf, NULL);

    // write to csv file
    csv_t *csv = csv_open("output.csv");
    csv_add_header(csv,"nodeId");
    csv_add_header(csv,"mag");
    csv_add_header(csv,"phase");
    csv_write_header(csv);

    for (usize i = 0; i < circuit->node_count; i++) {
        f64 dat[] = { (f64) i, circuit->nodes[i].potential, circuit->nodes[i].phase };
        csv_write_row(csv, dat);
    }

    del_circuit(circuit);
    b_free(&buf);
    csv_close(csv);
    return 0;
}
