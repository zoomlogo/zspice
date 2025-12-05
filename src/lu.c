#include <math.h>
#include <stdlib.h>

#include "error.h"
#include "lu.h"
#include "types.h"

#define A(i, j) MI(A, i, j, N)
#define L(i, j) MI(L, i, j, N)
#define U(i, j) MI(U, i, j, N)

static void swap_row(f64 *A, usize N, usize i, usize j) {
    if (i == j) return;

    for (usize k = 0; k < N; k++) {
        f64 temp = A(i, k);
        A(i, k) = A(j, k);
        A(j, k) = temp;
    }
}


static i32 lu_decompose(f64 *A, usize N, usize *p) {
    for (usize j = 0; j < N - 1; j++) {
        p[j] = j;
    }

    for (usize j = 0; j < N - 1; j++) {
        f64 biggest = 0;

        for (usize k = j; k < N; k++) {
            f64 sum = 0;
            for (usize l = j; l < N; l++)
                sum += fabs(A(k, l));

            if (sum < LU_TOLERANCE) return -2;

            f64 current = fabs(A(k, j)) / sum;
            if (current > biggest)
                biggest = current, p[j] = k;

        }

        if (biggest < LU_TOLERANCE) return ERR_SINGULAR;

        swap_row(A, N, j, p[j]);

        if (fabs(A(j, j)) < LU_TOLERANCE) return ERR_SINGULAR;

        for (usize i = j + 1; i < N; i++) {
            A(i, j) /= A(j, j);
            for (usize k = j + 1; k < N; k++)
                A(i, k) -= A(i, j) * A(j, k);
        }
    }

    if (fabs(A(N - 1, N - 1)) < LU_TOLERANCE) return ERR_SINGULAR;

    return OK;
}


static void low_solve(f64 *L, usize N, f64 *B, usize *p) {
    for (usize k = 0; k < N - 1; k++) {
        f64 temp = B[k];
        B[k] = B[p[k]];
        B[p[k]] = temp;
    }

    for (usize i = 1; i < N; i++) {
        for (usize j = 0; j < i; j++) {
            B[i] -= L(i, j) * B[j];
        }
    }
}


static i32 upp_solve(f64 *U, usize N, f64 *Y) {
    for (usize i = N - 1; i < N && i >= 0; i--) {
        for (usize j = i + 1; j < N; j++)
            Y[i] -= U(i, j) * Y[j];

        if (fabs(U(i, i)) < LU_TOLERANCE) return ERR_SINGULAR;

        Y[i] /= U(i, i);
    }

    return OK;
}


i32 lu_solve(f64 *A, usize N, f64 *B) {
    usize *p = (usize *) malloc((N - 1) * sizeof(usize));
    if (p == NULL) return ERR_MEM_ALLOC;

    i32 err = lu_decompose(A, N, p);
    if (err != 0) goto err_0;

    low_solve(A, N, B, p);
    err = upp_solve(A, N, B);

err_0:
    free(p);
    return err;
}

#undef A
#undef L
#undef U
