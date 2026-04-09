/**
 * @file zmth.h
 * @brief Helper math utils.
 */
#pragma once
#include <math.h>

#include "types.h"

#define MIN_TOL 1e-12    ///< Tolerance below which quantities will be considered 0.

/**
 * @brief Maximum of x, y.
 */
static inline f64 zmax(f64 x, f64 y)
{
	return x > y ? x : y;
}

static inline f64 zmin(f64 x, f64 y)
{
	return x < y ? x : y;
}

/**
 * @brief Returns true if a number is almost zero.
 */
static inline bool zalmost_zero(f64 x)
{
	return fabs(x) < MIN_TOL;
}

/**
 * @brief Clamped log10.
 */
static inline f64 zlog10(f64 x)
{
	if (zalmost_zero(x))
		return -12;  // log10(1e-12)
	return log10(x);
}

/**
 * @brief Clamp x between a and b.
 */
static inline f64 zclamp(f64 x, f64 a, f64 b)
{
	if (x < a)
		return a;
	if (x > b)
		return b;
	return x;
}

/**
 * @brief Standard junction limiting function to prevent numerical errors.
 *
 * If the solver guessed a too far jump above the critical region, we
 * force the new guess to be increase logarithmically using the formula:
 * \f[V_2 \leftarrow V_1 + V_T\ln\left(1 + \frac{V_2 - V_1}{V_T}\right).\f]
 *
 * @param V2 The new guess.
 * @param V1 The old guess.
 * @param V_T The thermal voltage.
 * @param Vcrit The critical voltage.
 * @returns Clamped new guess.
 */
static inline f64 zjlimit(f64 V2, f64 V1, f64 V_T, f64 Vcrit)
{
	if (V2 > Vcrit && V2 - V1 > 2 * V_T)
		return V1 + V_T * log(1 + (V2 - V1) / V_T);
	return V2;
}

static inline f64 zflimit(f64 V2, f64 V1, f64 V_T0)
{
	f64 V_ts_hi = fabs(2 * (V1 - V_T0)) + 2;
	f64 V_ts_lo = V_ts_hi / 2 + 2;
	f64 V_tox = V_T0 + 3.5;
	f64 dV = V2 - V1;

	if (V1 >= V_T0) {
		if (V1 >= V_tox) {
			if (dV > 0)
				V2 = zmin(V2, V1 + 2);
			else if (dV < -V_ts_hi)
				V2 = V1 - V_ts_hi;
		} else {
			if (dV > 0)
				V2 = zmin(V2, V1 + 2);
			else if (dV < -V_ts_lo)
				V2 = V1 - V_ts_lo;
		}
	} else {
		if (dV > 0 && V2 > V_T0)
			V2 = zmin(V2, V_T0 + 2);
	}
	return V2;
}
