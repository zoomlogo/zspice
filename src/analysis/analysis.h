#pragma once
#include "core/circuit.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "types.h"

error_e dc_solve_linear(circuit_t *circuit, sbuf_t *buffer ,env_t *env);
error_e dc_solve_non_linear(circuit_t *circuit, sbuf_t *buffer, env_t *env);

error_e ac_solve(circuit_t *circuit, sbuf_t *buffer, env_t *env);
// error_e ac_sweep(circuit_t *circuit, f64 start_freq, f64 stop_freq, usize num_points);
