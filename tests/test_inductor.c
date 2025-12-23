#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

void test_dc_stamp_inductor(void) {
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

void test_inductor(void) {
    BEGIN_TEST();

    test_dc_stamp_inductor();

    END_TEST();
}
