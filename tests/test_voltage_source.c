#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

void test_dc_stamp_voltage_source(void) {
    f64 A[9] = {0};
    f64 b[3] = {0};
    error_t err;

    component_t v = { VOLTAGE_SOURCE, 1, 0, .V.potential = 5, .solver_id = 2 };
    err = dc_stamp_voltage_source(3, A, b, &v);
    ASSERT(err == OK);
    ASSERTF(b[2], 5);
    ASSERTF(A[2], 1);
    ASSERTF(A[6], 1);
}

void test_voltage_source(void) {
    BEGIN_TEST();

    test_dc_stamp_voltage_source();

    END_TEST();
}
