#include <math.h>
#include <stdio.h>

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
    error_e err = OK;

    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            err = diode_linearize(c, env);
        } else if (c->type == BJT) {
            err = bjt_linearize(c, env);
        }
        if (err != OK) return err;
    }

    return OK;
}

error_e dc_update_guesses(circuit_t *circuit, sbuf_t *buffer) {
    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            usize n0 = c->id0;
            usize n1 = c->id1;

            f64 V_anode = n0 > 0 ? buffer->b[n0 - 1] : 0;
            f64 V_cathode = n1 > 0 ? buffer->b[n1 - 1] : 0;
            // compute new junction voltage
            f64 Vj = V_anode - V_cathode;

            c->D._Vj = c->D.Vj; // store the old value
            diode_limit(c, Vj, &c->D.Vj); // overwrite
        } else if (c->type == BJT) {
            usize n0 = c->id0;
            usize n1 = c->id1;
            usize n2 = c->id2;

            f64 Vb = n0 > 0 ? buffer->b[n0 - 1] : 0;
            f64 Ve = n1 > 0 ? buffer->b[n1 - 1] : 0;
            f64 Vc = n2 > 0 ? buffer->b[n2 - 1] : 0;

            // new guesses
            f64 Vbe = Vb - Ve;
            f64 Vbc = Vb - Vc;

            c->Q._Vbe = c->Q.Vbe;
            c->Q._Vbc = c->Q.Vbc;
            bjt_limit(c, Vbe, Vbc, &c->Q.Vbe, &c->Q.Vbc);
            printf("Vbe(%lf), Vbc(%lf)\n", c->Q.Vbe, c->Q.Vbc);
        }
    }

    return OK;
}


bool dc_check_convergence(circuit_t *circuit) {
    bool converged = true;

    for (usize i = 0; i < circuit->component_count; i++) {
        component_t *c = &circuit->components[i];
        if (c->type == DIODE) {
            // absolute convergence
            // TODO relative convergence
            converged &= fabs(c->D.Vj - c->D._Vj) < CONVERGENCE_TOLERANCE;
        } else if (c->type == BJT) {
            converged &= fabs(c->Q.Vbe - c->Q._Vbe) < CONVERGENCE_TOLERANCE;
            converged &= fabs(c->Q.Vbc - c->Q._Vbc) < CONVERGENCE_TOLERANCE;
        }
    }

    return converged;
}
