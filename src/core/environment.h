#pragma once
#include "types.h"
#include "util/error.h"

// Physical constants.
#define K_BOLTZMANN 1.380649e-23
#define K_CHARGE 1.602176e-19

// Stores information about the environment of the circuit.
typedef struct {
    // Temperature
    f64 temperature; // in Kelvin.
    f64 V_T; // thermal voltage

    // AC analysis
    f64 w; // angular frequency
    f64 f; // frequency
} env_t;

error_e e_init(env_t *env);
error_e e_set_temperature(env_t *env, f64 T);
