#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_current_source(void) {
    f64 A[4] = {0};
    f64 b[2] = {0};
    error_e err;
    env_t env; e_init(&env);

    component_t i = { CURRENT_SOURCE, 1, 0, .I.dc_offset = 5 };
    err = dc_stamp_current_source(2, A, b, &i, &env);
    ASSERT(err == OK);
    ASSERTF(b[0], 5);
}

static void test_ac_stamp_current_source(void) {
    c64 A[4] = {0};
    c64 b[2] = {0};
    error_e err;
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t i = { CURRENT_SOURCE, 1, 0, .I.max_current = 5, .I.frequency = NAN, .I.phase_offset = 90 };
    err = ac_stamp_current_source(2, A, b, &i, &env);
    ASSERT(err == OK);
    ASSERTC(b[0], 5 * J);
}

void test_current_source(void) {
    BEGIN_TEST();

    test_dc_stamp_current_source();
    test_ac_stamp_current_source();

    END_TEST();
}
