/**
 * @file environment.h
 */
#pragma once
#include "types.h"
#include "util/error.h"

// Physical constants.
#define K_BOLTZMANN 1.380649e-23 ///< The Boltzmann constant, \f(k\f).
#define K_CHARGE 1.602176e-19 ///< The elementary charge, \f(q\f).

/**
 * @brief Stores information about the environment of the circuit.
 */
typedef struct {
    // Temperature
    f64 temperature; ///< Global Temperature, \f(T\f), of the circuit in Kelvin.
    f64 V_T; ///< The corresponding Thermal Voltage, \f(V_T\f).

    // AC analysis
    f64 w; ///< The angular frequency, \f(\omega\f), for AC analysis.
    f64 f; ///< The frequency, \f(f\f), for AC analysis.
} env_t;

/**
 * @brief Initialize an environment.
 *
 * @param env The environment to initialize.
 * @returns OK on success.
 */
error_e e_init(env_t *env);
/**
 * @brief Sets the temperature of an environment.
 *
 * Converts Celsius to Kelvin and updates \f(V_T\f) also.
 * \f[ V_T = \frac{kT}q. \f]
 *
 * @param env The environment.
 * @param T Temperature in degrees Celsius.
 * @returns OK on success.
 */
error_e e_set_temperature(env_t *env, f64 T);
/**
 * @brief Set the frequency of the enviroment.
 *
 * Converts the frequency into angular frequency.
 * \f[ \omega = 2\pi f. \f]
 *
 * @param env The environment.
 * @param f The frequency, in Hertz.
 * @returns OK on success.
 */
error_e e_set_frequency(env_t *env, f64 f);
