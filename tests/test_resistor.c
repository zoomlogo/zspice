#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

static void test_dc_stamp_resistor(void) {
    f64 A[4] = {0};
    f64 b[2] = {0};
    error_t err;

    component_t r = { RESISTOR, 0, 1, .R.resistance = 100, .R.conductance = NAN };
    err = dc_stamp_resistor(2, A, b, &r);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERTF(A[0], 0.01);
    ASSERT(A[1] == 0 && A[2] == 0 && A[3] == 0);

    A[0] = 0;
    r.id0 = 1;
    r.id1 = 2;
    err = dc_stamp_resistor(2, A, b, &r);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERTF(A[0], 0.01);
    ASSERTF(A[1], -0.01);
    ASSERTF(A[2], -0.01);
    ASSERTF(A[3], 0.01);

    r.R.resistance = 0;
    r.R.conductance = NAN;
    err = dc_stamp_resistor(2, A, b, &r);
    ASSERT(err == ERR_INVALID_PARAM);
}

void test_resistor(void) {
    BEGIN_TEST();

    test_dc_stamp_resistor();

    END_TEST();
}
