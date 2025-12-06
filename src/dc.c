#include <string.h>

#include "dc.h"
#include "component/component.h"
#include "error.h"

error_t dc_solve(circuit_t *circuit) {
    if (circuit->dim == 0) return ERR_NOT_INIT;

    // reset memory
    memset(circuit->A, 0, circuit->dim * circuit->dim * sizeof(f64));
    memset(circuit->b, 0, circuit->dim * sizeof(f64));

    // setup matrix
    for (usize i = 0; i < circuit->component_count; i++) {
    }

    return OK;
}
