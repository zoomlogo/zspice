#include <math.h>

#include "component/component.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_resistor(void) {
    sbuf_t buf; b_init(2, false, &buf);
    error_e err;
    env_t env; e_init(&env);

    component_t r = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = NAN };
    err = dc_stamp_resistor(&buf, &r, &env);
    ASSERT(err == OK);
    ASSERT(buf.b[0] == 0 && buf.b[1] == 0);
    ASSERTF(buf.A[0], 0.01);
    ASSERT(buf.A[1] == 0 && buf.A[2] == 0 && buf.A[3] == 0);

    buf.A[0] = 0;
    r.id0 = 1;
    r.id1 = 2;
    err = dc_stamp_resistor(&buf, &r, &env);
    ASSERT(err == OK);
    ASSERT(buf.b[0] == 0 && buf.b[1] == 0);
    ASSERTF(buf.A[0], 0.01);
    ASSERTF(buf.A[1], -0.01);
    ASSERTF(buf.A[2], -0.01);
    ASSERTF(buf.A[3], 0.01);

    r.R.resistance = 0;
    r.R.conductance = NAN;
    err = dc_stamp_resistor(&buf, &r, &env);
    ASSERT(err == ERR_INVALID_PARAM);

    b_free(&buf);
}

static void test_ac_stamp_resistor(void) {
    sbuf_t buf; b_init(2, true, &buf);
    error_e err;
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t r = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = NAN };
    err = ac_stamp_resistor(&buf, &r, &env);
    ASSERT(err == OK);
    ASSERT(buf.zb[0] == 0 && buf.zb[1] == 0);
    ASSERTC(buf.zA[0], 0.01);
    ASSERT(buf.zA[1] == 0 && buf.zA[2] == 0 && buf.zA[3] == 0);

    buf.zA[0] = 0;
    r.id0 = 1;
    r.id1 = 2;
    err = ac_stamp_resistor(&buf, &r, &env);
    ASSERT(err == OK);
    ASSERT(buf.zb[0] == 0 && buf.zb[1] == 0);
    ASSERTC(buf.zA[0], 0.01);
    ASSERTC(buf.zA[1], -0.01);
    ASSERTC(buf.zA[2], -0.01);
    ASSERTC(buf.zA[3], 0.01);

    r.R.resistance = 0;
    r.R.conductance = NAN;
    err = ac_stamp_resistor(&buf, &r, &env);
    ASSERT(err == ERR_INVALID_PARAM);

    b_free(&buf);
}

void test_resistor(void) {
    BEGIN_TEST();

    test_dc_stamp_resistor();
    test_ac_stamp_resistor();

    END_TEST();
}
