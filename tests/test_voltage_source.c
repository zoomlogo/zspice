#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

static void test_dc_stamp_voltage_source(void) {
    f64 A[9] = {0};
    f64 b[3] = {0};
    error_e err;

    component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.dc_offset = 5 };
    err = dc_stamp_voltage_source(3, A, b, &v);
    ASSERT(err == OK);
    ASSERTF(b[2], 5);
    ASSERTF(A[2], 1);
    ASSERTF(A[6], 1);
}

static void test_ac_stamp_voltage_source(void) {
    c64 A[9] = {0};
    c64 b[3] = {0};
    error_e err;

    component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.max_voltage = 5, .V.frequency = NAN, .V.phase_offset = 90 };
    err = ac_stamp_voltage_source(3, A, b, &v, 40);
    ASSERT(err == OK);
    ASSERTC(b[2], 5 * J);
    ASSERTC(A[2], 1);
    ASSERTC(A[6], 1);
}

void test_voltage_source(void) {
    BEGIN_TEST();

    test_dc_stamp_voltage_source();
    test_ac_stamp_voltage_source();

    END_TEST();
}
