#include <math.h>

#include "util/error.h"

#include "environment.h"

error_e e_init(env_t *env) {
    env->f = 0;
    env->w = 0;
    e_set_temperature(env, 27);
    return OK;
}

error_e e_set_temperature(env_t *env, f64 T) {
    env->temperature = T + 273.15;
    env->V_T = K_BOLTZMANN * env->temperature / K_CHARGE;
    return OK;
}

error_e e_set_frequency(env_t *env, f64 f) {
    env->f = f;
    env->w = 2 * M_PI * env->f;
    return OK;
}
