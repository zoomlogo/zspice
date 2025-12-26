#pragma once
#include "core/circuit.h"
#include "util/error.h"

#include "types.h"

error_e dc_solve_linear(circuit_t *circuit);
error_e dc_solve_non_linear(circuit_t *circuit);

error_e ac_solve(circuit_t *circuit, f64 w);
error_e ac_sweep(circuit_t *circuit, f64 start_freq, f64 stop_freq, f64 step);
