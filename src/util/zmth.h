/**
 * @file zmth.h
 * @brief Helper math utils.
 */
#pragma once
#include <math.h>

#include "types.h"

#define MIN_TOL 1e-12 ///< Tolerance below which quantities will be considered 0.

/**
 * @brief Clamped log10.
 */
static inline f64 zlog10(f64 x) {
    if (x < MIN_TOL) return -12; // log10(1e-12)
    return log10(x);
}

/**
 * @brief Clamp x between a and b.
 */
static inline f64 zclamp(f64 x, f64 a, f64 b) {
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

/**
 * @brief Standard junction limiting function to prevent numerical errors.
 *
 * If the solver guessed a too far jump above the critical region, we
 * force the new guess to be increase logarithmically using the formula:
 * \f[V_2 \leftarrow V_2 + V_T\left(1 +\ln\left(\frac{V_2 - V_1}{V_T}\right)\right).\f]
 *
 * @param V2 The new guess.
 * @param V1 The old guess.
 * @param V_T The thermal voltage.
 * @param Vcrit The critical voltage.
 * @returns Clamped new guess.
 */
static inline f64 zjlimit(f64 V2, f64 V1, f64 V_T, f64 Vcrit) {
    if (V2 > Vcrit && V2 - V1 > 2 * V_T)
        return V2 + V_T * (1 + log((V2 - V1) / V_T));
    return V2;
}
