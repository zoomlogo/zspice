#include <math.h>

#include "component/component.h"
#include "error.h"
#include "test.h"
#include "test_def.h"

void test_dc_stamp_current_source(void) {
    f64 A[4] = {0};
    f64 b[2] = {0};
    error_t err;

    component_t i = { CURRENT_SOURCE, 1, 0, .I.current = 5 };
    err = dc_stamp_current_source(2, A, b, &i);
    ASSERT(err == OK);
    ASSERTF(b[0], 5);
}

void test_current_source(void) {
    BEGIN_TEST();

    test_dc_stamp_current_source();

    END_TEST();
}
