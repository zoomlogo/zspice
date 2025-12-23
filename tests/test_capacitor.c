#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

static void test_dc_stamp_capacitor(void) {
    f64 A[4] = {0};
    f64 b[2] = {0};
    error_e err;

    component_t c = { CAPACITOR, 0, 1, .C.capacitance = 0.01 };
    err = dc_stamp_capacitor(2, A, b, &c);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERT(A[0] == 0 && A[1] == 0 && A[2] == 0 && A[3] == 0);

    c.id0 = 1;
    c.id1 = 2;
    err = dc_stamp_capacitor(2, A, b, &c);
    ASSERT(err == OK);
    ASSERT(b[0] == 0 && b[1] == 0);
    ASSERTF(A[0], 0);
    ASSERTF(A[1], 0);
    ASSERTF(A[2], 0);
    ASSERTF(A[3], 0);
}

void test_capacitor(void) {
    BEGIN_TEST();

    test_dc_stamp_capacitor();

    END_TEST();
}
