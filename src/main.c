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

    const int n = 1;
    usize ids[n]; ids[0] = 2;
    ac_sweep_params_t params = { AC_SWEEP_DECADE, 1, 20000, 10, "output.csv", ids, n, 1};
    ac_sweep(circuit, &params, NULL);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
