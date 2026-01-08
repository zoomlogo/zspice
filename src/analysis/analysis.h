/**
 * @file analysis.h
 */
#pragma once
#include "core/circuit.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "types.h"

/**
 * @brief Solve a linear DC circuit.
 *
 * The resultant voltages are written in the circuit's nodes itself.
 *
 * @param circuit The circuit to solve. The circuit must have been
 *                initialized beforehand by calling c_calculate_dim().
 * @param buffer The solver buffer in which the MNA matrices resides.
 * @param env The environment of the circuit.
 * @returns OK on success.
 */
error_e dc_solve_linear(circuit_t *circuit, sbuf_t *buffer, env_t *env);
/**
 * @brief Solve a non-linear DC circuit.
 *
 * The resultant voltages are written in the circuit's nodes itself.
 *
 * @param circuit The circuit to solve. The circuit must have been
 *                initialized beforehand by calling c_calculate_dim().
 * @param buffer The solver buffer in which the MNA matrices resides.
 * @param env The environment of the circuit.
 * @returns OK on success.
 */
error_e dc_solve_non_linear(circuit_t *circuit, sbuf_t *buffer, env_t *env);

/**
 * @brief Solve an AC circuit for a single frequency.
 *
 * Assumes the circuit's operating point is already found.
 * The resultant voltages are written in the circuit's nodes itself.
 *
 * @param circuit The circuit to solve. The circuit must have been
 *                initialized beforehand by calling c_calculate_dim().
 * @param buffer The solver buffer in which the MNA matrices resides.
 * @param env The environment of the circuit. (Contains the frequency.)
 * @returns OK on success.
 */
error_e ac_solve(circuit_t *circuit, sbuf_t *buffer, env_t *env);
// error_e ac_sweep(circuit_t *circuit, f64 start_freq, f64 stop_freq, usize num_points);
