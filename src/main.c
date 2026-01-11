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

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);
    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t d1 = { DIODE, 2, 0 }; c_defaults(&d1); d1.D.V_break = 5;

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &d1);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    usize ids[] = {2};
    dc_sweep_params_t params = {SWEEP_LINEAR, 0, -10, 10, 100000, "output.csv", ids, 1};
    dc_sweep(circuit, &params, NULL);

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
