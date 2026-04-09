#include <math.h>

#include "component/component.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(dc_stamp_resistor)
{
	sbuf_t buf;
	b_init(2, false, &buf);
	error_e err;
	env_t env;
	e_init(&env);

	component_t r = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = NAN };
	TEST_EXPECT_DEFER(dc_stamp_resistor(&buf, &r, &env) == OK);
	TEST_EXPECT(buf.b[0] == 0 && buf.b[1] == 0);
	TEST_EXPECT_FLOAT(buf.A[0], 0.01);
	TEST_EXPECT(buf.A[1] == 0 && buf.A[2] == 0 && buf.A[3] == 0);

	buf.A[0] = 0;
	r.id0 = 1;
	r.id1 = 2;
	TEST_EXPECT_DEFER(dc_stamp_resistor(&buf, &r, &env) == OK);
	TEST_EXPECT(buf.b[0] == 0 && buf.b[1] == 0);
	TEST_EXPECT_FLOAT(buf.A[0], 0.01);
	TEST_EXPECT_FLOAT(buf.A[1], -0.01);
	TEST_EXPECT_FLOAT(buf.A[2], -0.01);
	TEST_EXPECT_FLOAT(buf.A[3], 0.01);

	r.R.resistance = 0;
	r.R.conductance = NAN;
	err = dc_stamp_resistor(&buf, &r, &env);
	TEST_EXPECT(err == ERR_INVALID_PARAM);

defer:
	b_free(&buf);
}

TEST_DEFINE(ac_stamp_resistor)
{
	sbuf_t buf;
	b_init(2, true, &buf);
	error_e err;
	env_t env;
	e_init(&env);
	e_set_frequency(&env, 40);

	component_t r = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = NAN };
	TEST_EXPECT_DEFER(ac_stamp_resistor(&buf, &r, &env) == OK);
	TEST_EXPECT(buf.zb[0] == 0 && buf.zb[1] == 0);
	TEST_EXPECT_FLOAT(creal(buf.zA[0]), 0.01);
	TEST_EXPECT_FLOAT(cimag(buf.zA[0]), 0.0);
	TEST_EXPECT(buf.zA[1] == 0 && buf.zA[2] == 0 && buf.zA[3] == 0);

	buf.zA[0] = 0;
	r.id0 = 1;
	r.id1 = 2;
	TEST_EXPECT_DEFER(ac_stamp_resistor(&buf, &r, &env) == OK);
	TEST_EXPECT(buf.zb[0] == 0 && buf.zb[1] == 0);
	TEST_EXPECT_FLOAT(creal(buf.zA[0]), 0.01);
	TEST_EXPECT_FLOAT(cimag(buf.zA[0]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[1]), -0.01);
	TEST_EXPECT_FLOAT(cimag(buf.zA[1]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[2]), -0.01);
	TEST_EXPECT_FLOAT(cimag(buf.zA[2]), 0.0);
	TEST_EXPECT_FLOAT(creal(buf.zA[3]), 0.01);
	TEST_EXPECT_FLOAT(cimag(buf.zA[3]), 0.0);

	r.R.resistance = 0;
	r.R.conductance = NAN;
	err = ac_stamp_resistor(&buf, &r, &env);
	TEST_EXPECT(err == ERR_INVALID_PARAM);

defer:
	b_free(&buf);
}

void test_resistor(void)
{
	TEST_BEGIN();

	TEST_RUN(dc_stamp_resistor);
	TEST_RUN(ac_stamp_resistor);

	TEST_END();
}
