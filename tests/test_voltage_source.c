#include <math.h>

#include "component/component.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_voltage_source(void) {
    sbuf_t buf; b_init(3, false, &buf);
    env_t env; e_init(&env);

    component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.dc_offset = 5 };
    ASSERT_OKC(dc_stamp_voltage_source(&buf, &v, &env));
    ASSERTF(buf.b[2], 5);
    ASSERTF(buf.A[2], 1);
    ASSERTF(buf.A[6], 1);

err:
    b_free(&buf);
}

static void test_ac_stamp_voltage_source(void) {
    sbuf_t buf; b_init(3, true, &buf);
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t v = { VOLTAGE_SOURCE, 1, 0, .solver_id = 2, .V.max_voltage = 5, .V.frequency = NAN, .V.phase_offset = 90 };
    ASSERT_OKC(ac_stamp_voltage_source(&buf, &v, &env));
    ASSERTC(buf.zb[2], 5 * J);
    ASSERTC(buf.zA[2], 1);
    ASSERTC(buf.zA[6], 1);

err:
    b_free(&buf);
}

void test_voltage_source(void) {
    BEGIN_TEST();

    test_dc_stamp_voltage_source();
    test_ac_stamp_voltage_source();

    END_TEST();
}
