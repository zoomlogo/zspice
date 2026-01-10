/**
 * @file analysis.h
 */
#pragma once
#include "core/circuit.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "types.h"

#define CONVERGENCE_TOLERANCE 1e-6
#define MAX_ITERATIONS 100

/**
 * @brief The different types of frequency sweeps.
 */
typedef enum {
    AC_SWEEP_LINEAR, ///< Standard Linear Sweep.
    AC_SWEEP_DECADE, ///< Standard Logarithmic Decade Sweep.
    AC_SWEEP_OCTAVE ///< Standard Octave Decade Sweep.
} ac_sweep_e;

/**
 * @brief The AC sweep parameters.
 */
typedef struct {
    // param part
    ac_sweep_e sweep_type; ///< The sweep type.
    f64 start_frequency; ///< Start frequency.
    f64 stop_frequency; ///< Stop frequency.
    usize steps; ///< Number of steps.

    // io part
    const char *filename; ///< Output CSV filename.
    usize *node_ids; ///< The list of node ids to output.
    usize n; ///< The length of `node_ids`.
    usize ref_node_id; ///< The id of the reference node to compute the gain w.r.t..
} ac_sweep_params_t;

error_e dc_linearize(circuit_t *circuit, env_t *env);
error_e dc_update_guesses(circuit_t *circuit, sbuf_t *buffer);
bool dc_check_convergence(circuit_t *circuit);

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
/**
 * @brief Sweep through frequencies for circuit.
 *
 * Writes a CSV file with the magnitude and phase
 * of selected nodes.
 *
 * @param circuit The circuit to sweep through.
 * @param params The parameters for the sweep.
 * @param env The environment of the circuit.
 * @returns OK on success.
 */
error_e ac_sweep(circuit_t *circuit, ac_sweep_params_t *params, env_t *env);
