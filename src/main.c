#include <stdio.h>
#include <math.h>

#include "analysis/analysis.h"
#include "core/circuit.h"
#include "core/sbuf.h"
#include "component/component.h"

#include "types.h"
#include "util/error.h"
#include "util/log.h"

i32 main(void) {
    sbuf_t buf;
    circuit_t *circuit = new_circuit();

    component_t v1 = { VOLTAGE_SOURCE, 1, 0 }; c_defaults(&v1);

    component_t r1 = { RESISTOR, 1, 2 }; c_defaults(&r1);
    component_t d1 = { DIODE, 2, 0 }; c_defaults(&d1);

    c_add_connection(circuit, &v1);
    c_add_connection(circuit, &r1);
    c_add_connection(circuit, &d1);

    c_calculate_dim(circuit);
    b_init(circuit->dim, true, &buf);

    usize nodes[] = {2};
    dc_sweep_params_t sweep_params = {
        SWEEP_LINEAR,
        0,
        -10,
        10,
        10000,
        "output.csv",
        nodes, 1
    };
    error_e err = dc_sweep(circuit, &sweep_params, NULL);
    if (err != OK) log_error("%s", err_str(err));

    del_circuit(circuit);
    b_free(&buf);
    return 0;
}
