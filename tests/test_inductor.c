#include <math.h>

#include "component/component.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(dc_stamp_inductor)
{
	sbuf_t buf;
	b_init(3, false, &buf);
	env_t env;
	e_init(&env);

	component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
	TEST_EXPECT_DEFER(dc_stamp_inductor(&buf, &l, &env) == OK);
	TEST_EXPECT_FLOAT(buf.b[2], 0);
	TEST_EXPECT_FLOAT(buf.A[2], 1);
	TEST_EXPECT_FLOAT(buf.A[6], 1);

defer:
	b_free(&buf);
}

TEST_DEFINE(ac_stamp_inductor)
{
	sbuf_t buf;
	b_init(3, true, &buf);
	env_t env;
	e_init(&env);
	e_set_frequency(&env, 40);

	component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
	TEST_EXPECT_DEFER(ac_stamp_inductor(&buf, &l, &env) == OK);
	TEST_EXPECT_FLOAT(creal(buf.zb[2]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zb[2]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[2]), 1.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[2]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[6]), 1.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[6]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[8]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[8]), -2 * M_PI * 40);

defer:
	b_free(&buf);
}

void test_inductor(void)
{
	TEST_BEGIN();

	TEST_RUN(dc_stamp_inductor);
	TEST_RUN(ac_stamp_inductor);

	TEST_END();
}
