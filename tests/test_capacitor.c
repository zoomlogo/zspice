#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(dc_stamp_capacitor)
{
	sbuf_t buf;
	b_init(2, false, &buf);
	env_t env;
	e_init(&env);

	component_t c = { CAPACITOR, 1, 2, .C.capacitance = 0.01 };
	TEST_EXPECT_DEFER(dc_stamp_capacitor(&buf, &c, &env) == OK);
	TEST_EXPECT(buf.b[0] == 0 && buf.b[1] == 0);
	TEST_EXPECT(buf.A[0] == 0 && buf.A[1] == 0 && buf.A[2] == 0 && buf.A[3] == 0);

defer:
	b_free(&buf);
}

TEST_DEFINE(ac_stamp_capacitor)
{
	sbuf_t buf;
	b_init(2, true, &buf);
	env_t env;
	e_init(&env);
	e_set_frequency(&env, 40);

	component_t c = { CAPACITOR, 1, 2, .C.capacitance = 0.01 };
	TEST_EXPECT_DEFER(ac_stamp_capacitor(&buf, &c, &env) == OK);
	TEST_EXPECT_FLOAT(creal(buf.zA[0]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[0]), 2 * M_PI * 40 * 0.01);
	TEST_EXPECT_FLOAT(creal(buf.zA[1]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[1]), -2 * M_PI * 40 * 0.01);
	TEST_EXPECT_FLOAT(creal(buf.zA[2]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[2]), -2 * M_PI * 40 * 0.01);
	TEST_EXPECT_FLOAT(creal(buf.zA[3]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[3]), 2 * M_PI * 40 * 0.01);

defer:
	b_free(&buf);
}

void test_capacitor(void)
{
	TEST_BEGIN();

	TEST_RUN(dc_stamp_capacitor);
	TEST_RUN(ac_stamp_capacitor);

	TEST_END();
}
