#include <stdlib.h>

#include "lu.h"
#include "types.h"

#include "test.h"
#include "test_def.h"

void test_lu(void) {
    BEGIN_TEST();

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

    f64 b0 = b[0] - 0.4;
    f64 b1 = b[1] - 1.4;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    for (usize i = 0; i < N; i++)
        free(A[i]);
    free(A);

    END_TEST();
}
