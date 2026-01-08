/**
 * @file sbuf.h
 */
#pragma once

#include "util/error.h"

#include "types.h"

/**
 * @brief Stores the matrix which is to be solved.
 */
typedef struct {
    usize dim; ///< The dimension of the matrix / vectors involved.

    // DC.
    f64 *A; ///< DC/Transient analysis. dim×dim matrix
    f64 *b; ///< DC/Transient analysis. dim vector

    // AC.
    c64 *zA; ///< AC analysis. dim×dim matrix.
    c64 *zb; ///< AC analysis. dim vector.
} sbuf_t;

/**
 * @brief Initialize and allocate memory for the solver buffer.
 *
 * @param dim The number of dimensions.
 * @param is_ac Should AC matrices be allocated?
 * @param solver_buffer The buffer to initialize.
 * @returns OK on success.
 */
error_e b_init(usize dim, bool is_ac, sbuf_t *solver_buffer);
/**
 * @brief Frees a buffer.
 *
 * @return OK on success.
 */
error_e b_free(sbuf_t *buffer);
