// unit testing utilities
#pragma once
#include <stdio.h>
#include "../src/types.h"

extern usize test_num, passed;

#define EPSILON 1e-6
#define ASSERT(cond) \
    ({ test_num++; \
    if ( cond ) passed++; \
    else { \
        printf("test failed @ %s:%d || (%s)\n", __FILE__, __LINE__, #cond); \
    } })
#define BEGIN_TEST() printf("entering %s...\n", __FILE__); test_num = 0, passed = 0
#define END_TEST() printf("%lu / %lu passed.\n", passed, test_num)
