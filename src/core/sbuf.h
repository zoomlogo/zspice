#pragma once

#include "util/error.h"

#include "types.h"

// Stores the matrix which is to be solved.
typedef struct {
    usize dim; // the dimension of the matrix / vectors involved

    // DC.
    f64 *A; // dim×dim matrix
    f64 *b; // dim vector

    // AC.
    c64 *zA;
    c64 *zb;
} sbuf_t;

error_e b_init(usize dim, bool is_ac, sbuf_t *solver_buffer);
error_e b_free(sbuf_t *buffer);
