#include <stdlib.h>

#include "util/error.h"
#include "util/lu.h"

#include "types.h"
#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(lu_simple_1)
{
	const usize N = 2;
	f64 A[N * N];

	A[0] = 2, A[1] = 3;
	A[2] = 1, A[3] = 4;

	f64 b[] = { 5, 6 };

	TEST_EXPECT_RETURN(r_lu_solve(A, N, b) == OK);

	TEST_EXPECT_FLOAT(b[0], 0.4);
	TEST_EXPECT_FLOAT(b[1], 1.4);
}

TEST_DEFINE(lu_simple_2)
{
	const usize N = 2;
	f64 A[N * N];

	A[0] = 1, A[1] = 1;
	A[2] = 1, A[3] = 2;

	f64 b[] = { 5, 7 };

	TEST_EXPECT_RETURN(r_lu_solve(A, N, b) == OK);

	TEST_EXPECT_FLOAT(b[0], 3);
	TEST_EXPECT_FLOAT(b[1], 2);
}

TEST_DEFINE(lu_pivoting)
{
	const usize N = 2;
	f64 A[N * N];

	A[0] = 0, A[1] = 1;
	A[2] = 1, A[3] = 0;

	f64 b[] = { 5, 7 };

	TEST_EXPECT_RETURN(r_lu_solve(A, N, b) == OK);

	TEST_EXPECT_FLOAT(b[0], 7.0);
	TEST_EXPECT_FLOAT(b[1], 5.0);
}

TEST_DEFINE(lu_singular)
{
	const usize N = 2;
	f64 A[N * N];

	A[0] = 1, A[1] = 1;
	A[2] = 2, A[3] = 2;

	f64 b[] = { 5, 10 };

	i32 err = r_lu_solve(A, N, b);
	TEST_EXPECT(err == ERR_SINGULAR);
}

void test_lu(void)
{
	TEST_BEGIN();

	TEST_RUN(lu_simple_1);
	TEST_RUN(lu_simple_2);
	TEST_RUN(lu_pivoting);
	TEST_RUN(lu_singular);

	TEST_END();
}
