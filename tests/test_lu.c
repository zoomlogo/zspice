#include <stdlib.h>

#include "util/error.h"
#include "util/lu.h"

#include "types.h"
#include "test.h"
#include "test_def.h"

static inline void test_lu_simple_1(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 2, A[1] = 3;
    A[2] = 1, A[3] = 4;

    f64 b[] = {5, 6};

    i32 err = r_lu_solve(A, N, b);
    ASSERT(err == OK);

    ASSERTF(b[0], 0.4);
    ASSERTF(b[1], 1.4);

    free(A);
}

static inline void test_lu_simple_2(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 1, A[1] = 1;
    A[2] = 1, A[3] = 2;

    f64 b[] = {5, 7};

    i32 err = r_lu_solve(A, N, b);
    ASSERT(err == OK);

    ASSERTF(b[0], 3);
    ASSERTF(b[1], 2);

    free(A);
}

static inline void test_lu_pivoting(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 0, A[1] = 1;
    A[2] = 1, A[3] = 0;

    f64 b[] = {5, 7};

    i32 err = r_lu_solve(A, N, b);
    ASSERT(err == OK);

    ASSERTF(b[0], 7.0);
    ASSERTF(b[1], 5.0);

    free(A);
}

static inline void test_lu_singular(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 1, A[1] = 1;
    A[2] = 2, A[3] = 2;

    f64 b[] = {5, 10};

    i32 err = r_lu_solve(A, N, b);
    ASSERT(err == ERR_SINGULAR);

    free(A);
}

void test_lu(void) {
    BEGIN_TEST();

    test_lu_simple_1();
    test_lu_simple_2();
    test_lu_pivoting();
    test_lu_singular();

    END_TEST();
}
