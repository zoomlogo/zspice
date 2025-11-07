#include <math.h>
#include <stdlib.h>

#include "lu.h"
#include "types.h"

void swap_row(f64 **A, usize N, usize i, usize j) {
    if (i == j) return;

    for (usize k = 0; k < N; k++) {
        f64 temp = A[i][k];
        A[i][k] = A[j][k];
        A[j][k] = temp;
    }
}


void lu_decompose(f64 **A, usize N, usize *p) {
    for (usize j = 0; j < N - 1; j++) {
        f64 biggest = 0;

        for (usize k = j; k < N; k++) {
            f64 sum = 0;
            for (usize l = j; l < N; l++)
                sum += fabs(A[k][l]);
            f64 current = fabs(A[k][j]) / sum;
            if (current > biggest)
                biggest = current, p[j] = k;
        }

        swap_row(A, N, j, p[j]);

        for (usize i = j + 1; i < N; i++) {
            A[i][j] /= A[j][j];
            for (usize k = j + 1; k < N; k++)
                A[i][k] -= A[i][j] * A[j][k];
        }
    }
}


void low_solve(f64 **L, usize N, f64 *B, usize *p) {
    for (usize k = 0; k < N - 1; k++) {
        f64 temp = B[k];
        B[k] = B[p[k]];
        B[p[k]] = temp;
    }

    for (usize i = 1; i < N; i++) {
        for (usize j = 0; j < i; j++) {
            B[i] -= L[i][j] * B[j];
        }
    }
}


void upp_solve(f64 **U, usize N, f64 *Y) {
    for (usize i = N - 1; i > -1; i--) {
        for (usize j = i + 1; j < N; j++)
            Y[i] -= U[i][j] * Y[j];
        Y[i] /= U[i][i];
    }
}


void solve(f64 **A, usize N, f64 *B) {
    usize *p = (usize *) malloc((N - 1) * sizeof(usize));
    lu_decompose(A, N, p);
    low_solve(A, N, B, p);
    upp_solve(A, N, B);
    free(p);
}
