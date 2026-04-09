#include <math.h>
#include <stdio.h>

#include "component/component.h"
#include "core/circuit.h"
#include "core/environment.h"
#include "types.h"
#include "util/error.h"
#include "util/sht_log.h"
#include "util/zmth.h"

#include "analysis.h"

error_e dc_linearize(circuit_t *circuit, env_t *env)
{
	if (circuit == NULL)
		return ERR_INVALID_ARG;
	if (env == NULL)
		env = &circuit->default_env;
	error_e err = OK;

	for (usize i = 0; i < circuit->component_count; i++) {
		component_t *c = &circuit->components[i];
		if (c->type == DIODE) {
			err = diode_linearize(c, env);
		} else if (c->type == BJT) {
			err = bjt_linearize(c, env);
		} else if (c->type == MOSFET) {
			err = mosfet_linearize(c, env);
		}
		if (err != OK)
			return err;
	}

	return OK;
}

error_e dc_update_guesses(circuit_t *circuit, sbuf_t *buffer)
{
	for (usize i = 0; i < circuit->component_count; i++) {
		component_t *c = &circuit->components[i];
		if (c->type == DIODE) {
			usize n0 = c->id0;
			usize n1 = c->id1;

			f64 V_anode = n0 > 0 ? buffer->b[n0 - 1] : 0;
			f64 V_cathode = n1 > 0 ? buffer->b[n1 - 1] : 0;
			// compute new junction voltage
			f64 Vj = V_anode - V_cathode;

			c->D._Vj = c->D.Vj;  // store the old value
			diode_limit(c, Vj, &c->D.Vj);  // overwrite
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
		} else if (c->type == MOSFET) {
			usize ng = c->id0;
			usize nd = c->id1;
			usize ns = c->id2;
			usize nb = c->id3;

			f64 Vg = ng > 0 ? buffer->b[ng - 1] : 0;
			f64 Vd = nd > 0 ? buffer->b[nd - 1] : 0;
			f64 Vs = ns > 0 ? buffer->b[ns - 1] : 0;
			f64 Vb = nb > 0 ? buffer->b[nb - 1] : 0;

			// new guesses
			f64 Vgs = Vg - Vs;
			f64 Vds = Vd - Vs;
			log_warn("TODO Vbs");

			c->M._Vgs = c->M.Vgs;
			c->M._Vds = c->M.Vds;
			mosfet_limit(c, Vgs, Vds, &c->M.Vgs, &c->M.Vds);
		}
	}

	return OK;
}

bool dc_check_convergence(circuit_t *circuit)
{
	bool converged = true;

	for (usize i = 0; i < circuit->component_count && converged; i++) {
		component_t *c = &circuit->components[i];
		if (c->type == DIODE) {
			converged &=
			    fabs(c->D.Vj - c->D._Vj) <
			    CONVERGENCE_TOLERANCE +
			    RELATIVE_TOLERANCE * fabs(zmax(c->D._Vj, c->D.Vj));
		} else if (c->type == BJT) {
			converged &=
			    fabs(c->Q.Vbe - c->Q._Vbe) <
			    CONVERGENCE_TOLERANCE +
			    RELATIVE_TOLERANCE *
			    fabs(zmax(c->Q._Vbe, c->Q.Vbe));
			converged &=
			    fabs(c->Q.Vbc - c->Q._Vbc) <
			    CONVERGENCE_TOLERANCE +
			    RELATIVE_TOLERANCE *
			    fabs(zmax(c->Q._Vbc, c->Q.Vbc));
		} else if (c->type == MOSFET) {
			converged &=
			    fabs(c->M.Vgs - c->M._Vgs) <
			    CONVERGENCE_TOLERANCE +
			    RELATIVE_TOLERANCE *
			    fabs(zmax(c->M._Vgs, c->M.Vgs));
			converged &=
			    fabs(c->M.Vds - c->M._Vds) <
			    CONVERGENCE_TOLERANCE +
			    RELATIVE_TOLERANCE *
			    fabs(zmax(c->M._Vds, c->M.Vds));
			log_warn("TODO Vbs");
		}
	}

	return converged;
}
