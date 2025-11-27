#include <stdlib.h>
#include <stdio.h>

#include "lu.h"
#include "types.h"

int main() {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    A[0][0] = 2, A[0][1] = 3;
    A[1][0] = 1, A[1][1] = 4;

    f64 b[] = {5, 6};

    solve(A, N, b);

    for (usize i = 0; i < N; i++)
        free(A[i]);
    free(A);

    if (b[0] == 0.4 && b[1] == 1.4) {
        return -1;
    } else {
        return 0;
    }
}
