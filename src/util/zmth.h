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
