// unit testing utilities
#pragma once
#include <stdio.h>
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
#define ASSERTF(var, val) ASSERT(val - EPSILON < var && var < val + EPSILON)
#define BEGIN_TEST() printf("entering %s...\n", __FILE__); test_num = 0, passed = 0
#define END_TEST() printf("\033[32m%lu / %lu passed.\033[0m\n", passed, test_num)
