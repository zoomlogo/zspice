#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(dc_stamp_voltage_source)
{
	sbuf_t buf;
	b_init(3, false, &buf);
	env_t env;
	e_init(&env);

	component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.dc_offset = 5 };
	TEST_EXPECT_DEFER(dc_stamp_voltage_source(&buf, &v, &env) == OK);
	TEST_EXPECT_FLOAT(buf.b[2], 5);
	TEST_EXPECT_FLOAT(buf.A[2], 1);
	TEST_EXPECT_FLOAT(buf.A[6], 1);

defer:
	b_free(&buf);
}

TEST_DEFINE(ac_stamp_voltage_source)
{
	sbuf_t buf;
	b_init(3, true, &buf);
	env_t env;
	e_init(&env);
	e_set_frequency(&env, 40);

	component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.max_voltage = 5, .V.frequency = NAN, .V.phase_offset = 90 };
	TEST_EXPECT_DEFER(ac_stamp_voltage_source(&buf, &v, &env) == OK);
	TEST_EXPECT_FLOAT(creal(buf.zb[2]), 0.0);
	TEST_EXPECT_FLOAT(cimag(buf.zb[2]), 5.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[2]), 1.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[2]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[6]), 1.0);
	TEST_EXPECT_FLOAT(cimag(buf.zA[6]), 0.0);

defer:
	b_free(&buf);
}

void test_voltage_source(void)
{
	TEST_BEGIN();

	TEST_RUN(dc_stamp_voltage_source);
	TEST_RUN(ac_stamp_voltage_source);

	TEST_END();
}
