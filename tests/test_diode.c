#include <stdio.h>

#include "component/component.h"
#include "util/log.h"

#include "test_def.h"
#include "sht_test.h"

void test_diode(void)
{
	TEST_BEGIN();

	// note that the model is incomplete because of missing parameters
	// which require internal nodes. (this will be handled in the parser step)
	component_t D_1N4148 = { DIODE, 1, 2 };
	c_defaults(&D_1N4148);
	D_1N4148.D.Is = 2.682e-9;
	D_1N4148.D.N = 1.836;
	D_1N4148.D.V_break = 100;
	D_1N4148.D.Vj = 0.6;

	env_t env = { 0 };
	e_init(&env);
	TEST_EXPECT_RETURN(diode_linearize(&D_1N4148, &env) == OK);

	TEST_EXPECT_FLOAT(D_1N4148.D.i_eq, -0.009581);
	TEST_EXPECT_FLOAT(D_1N4148.D.g_eq, 0.017341);

	diode_limit(&D_1N4148, 1, &D_1N4148.D.Vj);
	TEST_EXPECT(D_1N4148.D.Vj < 0.7);
	diode_limit(&D_1N4148, -2, &D_1N4148.D.Vj);
	TEST_EXPECT_FLOAT(D_1N4148.D.Vj, -2);
	diode_limit(&D_1N4148, -102, &D_1N4148.D.Vj);
	TEST_EXPECT(D_1N4148.D.Vj > -100);

	TEST_END();
}
