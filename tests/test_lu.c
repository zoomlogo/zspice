#include <stdlib.h>

#include "error.h"
#include "lu.h"
#include "types.h"

#include "test.h"
#include "test_def.h"

static inline void test_lu_simple_1(void) {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    A[0][0] = 2, A[0][1] = 3;
    A[1][0] = 1, A[1][1] = 4;

    f64 b[] = {5, 6};

    i32 err = solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 0.4;
    f64 b1 = b[1] - 1.4;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    for (usize i = 0; i < N; i++)
        free(A[i]);
    free(A);
}

static inline void test_lu_simple_2(void) {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    A[0][0] = 1, A[0][1] = 1;
    A[1][0] = 1, A[1][1] = 2;

    f64 b[] = {5, 7};

    i32 err = solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 3.0;
    f64 b1 = b[1] - 2.0;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    for (usize i = 0; i < N; i++)
        free(A[i]);
    free(A);
}

static inline void test_lu_pivoting(void) {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    A[0][0] = 0, A[0][1] = 1;
    A[1][0] = 1, A[1][1] = 0;

    f64 b[] = {5, 7};

    i32 err = solve(A, N, b);
    ASSERT(err == OK);

    f64 b0 = b[0] - 7.0;
    f64 b1 = b[1] - 5.0;
    ASSERT(-EPSILON < b0 && b0 < EPSILON);
    ASSERT(-EPSILON < b1 && b1 < EPSILON);

    for (usize i = 0; i < N; i++)
        free(A[i]);
    free(A);
}

static inline void test_lu_singular(void) {
    const usize N = 2;
    f64 **A = (f64 **) calloc(N, sizeof(f64));
    for (usize i = 0; i < N; i++) {
        A[i] = (f64 *) calloc(N, sizeof(f64));
        A[i][i] = 1;
    }

    A[0][0] = 1, A[0][1] = 1;
    A[1][0] = 2, A[1][1] = 2;

    f64 b[] = {5, 10};

    i32 err = solve(A, N, b);
    ASSERT(err == ERR_SINGULAR);

    for (usize i = 0; i < N; i++)
        free(A[i]);
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
