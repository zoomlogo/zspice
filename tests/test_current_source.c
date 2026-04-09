#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(dc_stamp_current_source)
{
	sbuf_t buf;
	b_init(2, false, &buf);
	env_t env;
	e_init(&env);

	component_t i = { CURRENT_SOURCE, 1, 0,.I.dc_offset = 5 };
	TEST_EXPECT_DEFER(dc_stamp_current_source(&buf, &i, &env) == OK);
	TEST_EXPECT_FLOAT(buf.b[0], 5);

defer:
	b_free(&buf);
}

TEST_DEFINE(ac_stamp_current_source)
{
	sbuf_t buf;
	b_init(2, true, &buf);
	env_t env;
	e_init(&env);
	e_set_frequency(&env, 40);

	component_t i = { CURRENT_SOURCE, 1, 0,.I.max_current = 5,.I.frequency =
		    NAN,.I.phase_offset = 90 };
	TEST_EXPECT_DEFER(ac_stamp_current_source(&buf, &i, &env) == OK);
	TEST_EXPECT_FLOAT(creal(buf.zb[0]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zb[0]), 5.0);

defer:
	b_free(&buf);
}

void test_current_source(void)
{
	TEST_BEGIN();

	TEST_RUN(dc_stamp_current_source);
	TEST_RUN(ac_stamp_current_source);

	TEST_END();
}
