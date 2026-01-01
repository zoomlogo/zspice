#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_capacitor(void) {
    f64 A[4] = {0};
    f64 b[2] = {0};
    error_e err;
    env_t env; e_init(&env);

    component_t c = { CAPACITOR, 0, 1, .C.capacitance = 0.01 };
    err = dc_stamp_capacitor(2, A, b, &c, &env);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERT(A[0] == 0 && A[1] == 0 && A[2] == 0 && A[3] == 0);

    c.id0 = 1;
    c.id1 = 2;
    err = dc_stamp_capacitor(2, A, b, &c, &env);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERTF(A[0], 0);
    ASSERTF(A[1], 0);
    ASSERTF(A[2], 0);
    ASSERTF(A[3], 0);
}

static void test_ac_stamp_capacitor(void) {
    c64 A[4] = {0};
    c64 b[2] = {0};
    error_e err;
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t c = { CAPACITOR, 1, 2, .C.capacitance = 0.01 };
    err = ac_stamp_capacitor(2, A, b, &c, &env);
    ASSERT(err == OK);
    ASSERTC(A[0], J * 2 * M_PI * 40 * 0.01);
    ASSERTC(A[1], -J * 2 * M_PI * 40 * 0.01);
    ASSERTC(A[2], -J * 2 * M_PI * 40 * 0.01);
    ASSERTC(A[3], J * 2 * M_PI * 40 * 0.01);
}

void test_capacitor(void) {
    BEGIN_TEST();

    test_dc_stamp_capacitor();
    test_ac_stamp_capacitor();

    END_TEST();
}
