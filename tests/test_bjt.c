#include <stdio.h>

#include "component/component.h"
#include "util/sht_log.h"

#include "test_def.h"
#include "sht_test.h"

void test_bjt(void)
{
	TEST_BEGIN();

	component_t Q_2N2222 = { BJT, 1, 2, 3 };
	c_defaults(&Q_2N2222);
	Q_2N2222.Q.Is = 1.434e-14;
	Q_2N2222.Q.Bf = 256;
	Q_2N2222.Q.Br = 6.092;
	Q_2N2222.Q.Va = 74.03;
	Q_2N2222.Q.Cj0e = 22.01e-12;
	Q_2N2222.Q.phi_e = 0.75;
	Q_2N2222.Q.m_e = 0.377;
	Q_2N2222.Q.Cj0c = 7.306e-12;
	Q_2N2222.Q.phi_c = 0.75;
	Q_2N2222.Q.m_c = 0.342;
	Q_2N2222.Q.tau_f = 411.1e-12;
	Q_2N2222.Q.tau_r = 46.91e-9;

	env_t env = { 0 };
	e_init(&env);
	TEST_EXPECT_RETURN(bjt_linearize(&Q_2N2222, &env) == OK);

	TEST_EXPECT_FLOAT(Q_2N2222.Q.g_pi, 0.001228);
	TEST_EXPECT_FLOAT(Q_2N2222.Q.g_mu, 0.000000);
	TEST_EXPECT_FLOAT(Q_2N2222.Q.g_mf, 0.317341);
	TEST_EXPECT_FLOAT(Q_2N2222.Q.g_mr, 0.000000);
	TEST_EXPECT_FLOAT(Q_2N2222.Q.g_o, 0.000110);
	// these numbers are on the order of 10^-10 and 10^-12 so we can't really check for them
	TEST_EXPECT_FLOAT(Q_2N2222.Q.c_be, 0.000000);
	TEST_EXPECT_FLOAT(Q_2N2222.Q.c_bc, 0.000000);

	bjt_limit(&Q_2N2222, 10, 10, &Q_2N2222.Q.Vbe, &Q_2N2222.Q.Vbc);
	TEST_EXPECT(Q_2N2222.Q.Vbe < 0.9);
	TEST_EXPECT(Q_2N2222.Q.Vbc < 0.8);

	TEST_END();
}
