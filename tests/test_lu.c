#include <stdlib.h>

#include "error.h"
#include "lu.h"
#include "types.h"

#include "test.h"
#include "test_def.h"

static inline void test_lu_simple_1(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 2, A[1] = 3;
    A[2] = 1, A[3] = 4;

    f64 b[] = {5, 6};

    i32 err = lu_solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 0.4;
    f64 b1 = b[1] - 1.4;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    free(A);
}

static inline void test_lu_simple_2(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 1, A[1] = 1;
    A[2] = 1, A[3] = 2;

    f64 b[] = {5, 7};

    i32 err = lu_solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 3.0;
    f64 b1 = b[1] - 2.0;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    free(A);
}

static inline void test_lu_pivoting(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 0, A[1] = 1;
    A[2] = 1, A[3] = 0;

    f64 b[] = {5, 7};

    i32 err = lu_solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 7.0;
    f64 b1 = b[1] - 5.0;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    free(A);
}

static inline void test_lu_singular(void) {
    const usize N = 2;
    f64 *A = (f64 *) calloc(N * N, sizeof(f64));

    A[0] = 1, A[1] = 1;
    A[2] = 2, A[3] = 2;

    f64 b[] = {5, 10};

    i32 err = lu_solve(A, N, b);
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
