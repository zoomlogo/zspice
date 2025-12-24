#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

static void test_dc_stamp_inductor(void) {
    f64 A[9] = {0};
    f64 b[3] = {0};
    error_e err;

    component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
    err = dc_stamp_inductor(3, A, b, &l);
    ASSERT(err == OK);
    ASSERTF(b[2], 0);
    ASSERTF(A[2], 1);
    ASSERTF(A[6], 1);
}

static void test_ac_stamp_inductor(void) {
    c64 A[9] = {0};
    c64 b[3] = {0};
    error_e err;

    component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
    err = ac_stamp_inductor(3, A, b, &l, 40);
    ASSERT(err == OK);
    ASSERTC(b[2], 0);
    ASSERTC(A[2], 1);
    ASSERTC(A[6], 1);
    ASSERTC(A[8], J * 40);
}

void test_inductor(void) {
    BEGIN_TEST();

    test_dc_stamp_inductor();
    test_ac_stamp_inductor();

    END_TEST();
}
