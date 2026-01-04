#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_current_source(void) {
    sbuf_t buf; b_init(2, false, &buf);
    error_e err;
    env_t env; e_init(&env);

    component_t i = { CURRENT_SOURCE, 1, 0, .I.dc_offset = 5 };
    err = dc_stamp_current_source(&buf, &i, &env);
    ASSERT(err == OK);
    ASSERTF(buf.b[0], 5);

    b_free(&buf);
}

static void test_ac_stamp_current_source(void) {
    sbuf_t buf; b_init(2, true, &buf);
    error_e err;
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t i = { CURRENT_SOURCE, 1, 0, .I.max_current = 5, .I.frequency = NAN, .I.phase_offset = 90 };
    err = ac_stamp_current_source(&buf, &i, &env);
    ASSERT(err == OK);
    ASSERTC(buf.zb[0], 5 * J);

    b_free(&buf);
}

void test_current_source(void) {
    BEGIN_TEST();

    test_dc_stamp_current_source();
    test_ac_stamp_current_source();

    END_TEST();
}
