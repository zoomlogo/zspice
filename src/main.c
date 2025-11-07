#include <stdio.h>
#include <stdlib.h>

#include "lu.h"
#include "types.h"

i32 main(void) {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    M_PRINT((f64 *) A, N);

    return 0;
}
