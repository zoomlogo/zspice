#include <stdlib.h>

#include "sbuf.h"

error_e b_init(usize dim, bool is_ac, sbuf_t *solver_buffer) {
    if (solver_buffer == NULL) return ERR_INVALID_ARG;

    solver_buffer->dim = dim;
    if (solver_buffer->dim == 0) return ERR_NOT_INIT;

    solver_buffer->zA = NULL;
    solver_buffer->zb = NULL;

    // Initialize AC matrices.
    if (is_ac) {
        solver_buffer->zA = (c64 *) calloc(solver_buffer->dim * solver_buffer->dim, sizeof(c64));
        if (solver_buffer->zA == NULL) goto err_0;

        solver_buffer->zb = (c64 *) calloc(solver_buffer->dim, sizeof(c64));
        if (solver_buffer->zb == NULL) goto err_1;
    }

    // Initialize these matrices regardless of analysis type.
    // Since we need to find DC op-point before AC analysis.
    solver_buffer->A = (f64 *) calloc(solver_buffer->dim * solver_buffer->dim, sizeof(f64));
    if (solver_buffer->A == NULL) goto err_2;

    solver_buffer->b = (f64 *) calloc(solver_buffer->dim, sizeof(f64));
    if (solver_buffer->b == NULL) goto err_3;

    return OK;

err_3:
    free(solver_buffer->A);
err_2:
    free(solver_buffer->zb);
err_1:
    free(solver_buffer->zA);
err_0:
    return ERR_MEM_ALLOC;
}

error_e b_free(sbuf_t *buffer) {
    free(buffer->A);
    free(buffer->b);
    free(buffer->zA);
    free(buffer->zb);
    return OK;
}
