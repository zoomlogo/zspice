#include <stdio.h>

#include "types.h"

#define SHT_TEST_CONTAINS_MAIN
#include "test_def.h"
#include "sht_test.h"

i32 main(void)
{
#define X(f) test_##f();
	TESTS(X)
#undef X
	TEST_SUMMARY();
	return 0;
}
