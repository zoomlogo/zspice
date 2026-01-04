#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_capacitor(void) {
    sbuf_t buf; b_init(2, false, &buf);
    error_e err;
    env_t env; e_init(&env);

    component_t c = { CAPACITOR, 1, 2, .C.capacitance = 0.01 };
    err = dc_stamp_capacitor(&buf, &c, &env);
    ASSERT(err == OK);
    ASSERT(buf.b[0] == 0 && buf.b[1] == 0);
    ASSERT(buf.A[0] == 0 && buf.A[1] == 0 && buf.A[2] == 0 && buf.A[3] == 0);

    b_free(&buf);
}

static void test_ac_stamp_capacitor(void) {
    sbuf_t buf; b_init(2, true, &buf);
    error_e err;
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t c = { CAPACITOR, 1, 2, .C.capacitance = 0.01 };
    err = ac_stamp_capacitor(&buf, &c, &env);
    ASSERT(err == OK);
    ASSERTC(buf.zA[0], J * 2 * M_PI * 40 * 0.01);
    ASSERTC(buf.zA[1], -J * 2 * M_PI * 40 * 0.01);
    ASSERTC(buf.zA[2], -J * 2 * M_PI * 40 * 0.01);
    ASSERTC(buf.zA[3], J * 2 * M_PI * 40 * 0.01);

    b_free(&buf);
}

void test_capacitor(void) {
    BEGIN_TEST();

    test_dc_stamp_capacitor();
    test_ac_stamp_capacitor();

    END_TEST();
}
