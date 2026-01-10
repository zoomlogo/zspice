#include <math.h>

#include "component/component.h"
#include "core/circuit.h"
#include "core/environment.h"
#include "types.h"
#include "util/error.h"
#include "util/zmth.h"

#include "analysis.h"

error_e dc_linearize(circuit_t *circuit, env_t *env) {
    if (circuit == NULL) return ERR_INVALID_ARG;
    if (env == NULL) env = &circuit->default_env;

    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            diode_linearize(c, env);
        }
    }

    return OK;
}

error_e dc_update_guesses(circuit_t *circuit, sbuf_t *buffer) {
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            // new guess
            usize n0 = c->id0;
            usize n1 = c->id1;
            f64 V_anode = c->id0 > 0 ? buffer->b[n0 - 1] : 0;
            f64 V_cathode = c->id1 > 0 ? buffer->b[n1 - 1] : 0;
            f64 Vj = V_anode - V_cathode;
            c->D._Vj = c->D.Vj;
            c->D.Vj = diode_limit(c, Vj);
        }
    }

    return OK;
}


bool dc_check_convergence(circuit_t *circuit) {
    bool converged = true;

    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            converged &= fabs(c->D.Vj - c->D._Vj) < CONVERGENCE_TOLERANCE;
        }
    }

    return converged;
}
