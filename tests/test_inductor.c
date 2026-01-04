#include <math.h>

#include "component/component.h"
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "test.h"
#include "test_def.h"

static void test_dc_stamp_inductor(void) {
    sbuf_t buf; b_init(3, false, &buf);
    env_t env; e_init(&env);

    component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
    ASSERT_OKC(dc_stamp_inductor(&buf, &l, &env));
    ASSERTF(buf.b[2], 0);
    ASSERTF(buf.A[2], 1);
    ASSERTF(buf.A[6], 1);

err:
    b_free(&buf);
}

static void test_ac_stamp_inductor(void) {
    sbuf_t buf; b_init(3, true, &buf);
    env_t env; e_init(&env); e_set_frequency(&env, 40);

    component_t l = { INDUCTOR, 1, 0, .solver_id = 2, .L.inductance = 1 };
    ASSERT_OKC(ac_stamp_inductor(&buf, &l, &env));
    ASSERTC(buf.zb[2], 0);
    ASSERTC(buf.zA[2], 1);
    ASSERTC(buf.zA[6], 1);
    ASSERTC(buf.zA[8], J * 2 * M_PI * 40);

err:
    b_free(&buf);
}

void test_inductor(void) {
    BEGIN_TEST();

    test_dc_stamp_inductor();
    test_ac_stamp_inductor();

    END_TEST();
}
