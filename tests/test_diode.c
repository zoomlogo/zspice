#include <stdio.h>

#include "component/component.h"

#include "test_def.h"
#include "test.h"

void test_diode(void)
{
	BEGIN_TEST();

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
	ASSERT_OKR(diode_linearize(&D_1N4148, &env));

	ASSERTF(D_1N4148.D.i_eq, -0.009581);
	ASSERTF(D_1N4148.D.g_eq, 0.0174);

	diode_limit(&D_1N4148, 1, &D_1N4148.D.Vj);
	ASSERT(D_1N4148.D.Vj < 0.7);
	diode_limit(&D_1N4148, -2, &D_1N4148.D.Vj);
	ASSERTF(D_1N4148.D.Vj, -2);
	diode_limit(&D_1N4148, -102, &D_1N4148.D.Vj);
	ASSERT(D_1N4148.D.Vj > -100);

	END_TEST();
}
