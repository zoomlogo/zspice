// unit testing utilities
#pragma once
#include <stdio.h>
#include <math.h>
#include "../src/types.h"

extern usize test_num, passed, net_failed;

#define EPSILON 1e-6
#define ASSERT(cond) \
    ({ test_num++; \
    if ( cond ) passed++; \
    else { \
        net_failed++; \
        printf("\033[31mtest failed\033[0m @ \033[34m%s\033[0m:\033[35m%d\033[0m || (%s)\n", __FILE__, __LINE__, #cond); \
    } })
#define ASSERT_OKR(expr) do { \
        error_e _err = (expr); \
        if (_err != OK) { \
            printf("\033[31merror occurred\033[0m @ \033[34m%s\033[0m:\033[35m%d\033[0m || (%s)\n", __FILE__, __LINE__, err_str(_err)); \
            return; \
        } \
    } while (0)
#define ASSERT_OKC(expr) do { \
        error_e _err = (expr); \
        if (_err != OK) { \
            printf("\033[31merror occurred\033[0m @ \033[34m%s\033[0m:\033[35m%d\033[0m || (%s)\n", __FILE__, __LINE__, err_str(_err)); \
            goto err; \
        } \
    } while (0)
#define ASSERTF(var, val) ASSERT(fabs((var) - (val)) < EPSILON)
#define ASSERTC(var, val) ASSERT(cabs((var) - (val)) < EPSILON)
#define BEGIN_TEST() printf("entering %s...\n", __FILE__); test_num = 0, passed = 0
#define END_TEST() printf("\033[32m%lu / %lu passed.\033[0m\n", passed, test_num)
