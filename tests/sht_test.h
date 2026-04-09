/*****      test - Testing suite         *****/
/***** https://codeberg.org/zoomlogo/sht *****/
// Version: v0.1
#ifndef SHT_TEST_DEFINED_H
#define SHT_TEST_DEFINED_H

#include <stddef.h>
#include <stdio.h>
#include <math.h>

extern size_t sht_test_pass;
extern size_t sht_test_fail;
extern size_t sht_test_net_pass;
extern size_t sht_test_net_fail;

#ifndef SHT_TEST_NO_COLORS
#define SHT_TEST_RED "\033[31m"
#define SHT_TEST_GREEN "\033[32m"
#define SHT_TEST_YELLOW "\033[33m"
#define SHT_TEST_BLUE "\033[34m"
#define SHT_TEST_RESET "\033[0m"
#else
#define SHT_TEST_RED
#define SHT_TEST_GREEN
#define SHT_TEST_YELLOW
#define SHT_TEST_BLUE
#define SHT_TEST_RESET
#endif

#ifndef SHT_TEST_FLOAT_TOLERANCE
#define SHT_TEST_FLOAT_TOLERANCE 1e-6
#endif

#ifndef SHT_TEST_DEFER_LABEL
#define SHT_TEST_DEFER_LABEL defer
#endif

#ifndef SHT_TEST_FABS
#define SHT_TEST_FABS fabs
#endif

#ifdef SHT_STRIP_PREFIX
#define TEST_DEFINE SHT_TEST_DEFINE
#define TEST_RUN SHT_TEST_RUN
#define TEST_BEGIN SHT_TEST_BEGIN
#define TEST_END SHT_TEST_END
#define TEST_SUMMARY SHT_TEST_SUMMARY
#define TEST_EXPECT SHT_TEST_EXPECT
#define TEST_EXPECT_FLOAT SHT_TEST_EXPECT_FLOAT
#define TEST_EXPECT_RETURN SHT_TEST_EXPECT_RETURN
#define TEST_EXPECT_DEFER SHT_TEST_EXPECT_DEFER
#endif

#define SHT_TEST_DEFINE(name) static void sht_test_##name(void)
#define SHT_TEST_RUN(name) \
do {                                                                       \
        printf("  running " SHT_TEST_YELLOW #name SHT_TEST_RESET "...\n"); \
        sht_test_##name();                                                 \
} while (0)

#define SHT_TEST_BEGIN() \
do {                                                                       \
        printf("entering " SHT_TEST_BLUE __FILE__ SHT_TEST_RESET "...\n"); \
        sht_test_pass = 0;                                                 \
        sht_test_fail = 0;                                                 \
} while (0)

#define SHT_TEST_END() \
do {                                                                                      \
        printf("done: [" SHT_TEST_GREEN "%zu " SHT_TEST_RED "%zu" SHT_TEST_RESET "]\n\n", \
               sht_test_pass, sht_test_fail);                                             \
        sht_test_net_pass += sht_test_pass;                                               \
        sht_test_net_fail += sht_test_fail;                                               \
} while (0)

#define SHT_TEST_SUMMARY() \
do {                                                                                                              \
        printf("FINISHED: [" SHT_TEST_GREEN "%zu " SHT_TEST_RED "%zu " SHT_TEST_BLUE "%zu" SHT_TEST_RESET "]\n",  \
               sht_test_net_pass, sht_test_net_fail, sht_test_net_pass + sht_test_net_fail);                      \
        if (sht_test_net_fail > 0)                                                                                \
                printf(SHT_TEST_RED "FAILURES: %zu" SHT_TEST_RESET "\n", sht_test_net_fail);                      \
        else                                                                                                      \
                printf(SHT_TEST_GREEN "EVERYTHING PASSED.\n" SHT_TEST_RESET);                                     \
} while (0)


#define SHT_TEST_EXPECT(condition) \
do {                                                                                                               \
        if (condition) {                                                                                           \
                sht_test_pass++;                                                                                   \
        }                                                                                                          \
        else {                                                                                                     \
                sht_test_fail++;                                                                                   \
                printf(SHT_TEST_RED "[FAIL]" SHT_TEST_RESET " @ " __FILE__ ":%d || (" #condition ")\n", __LINE__); \
        }                                                                                                          \
} while (0)

#define SHT_TEST_EXPECT_FLOAT(expression, expected_value) \
        SHT_TEST_EXPECT(SHT_TEST_FABS((expression) - (expected_value)) <= SHT_TEST_FLOAT_TOLERANCE)

#define SHT_TEST_EXPECT_RETURN(condition) \
do {                                                                           \
        if (condition) {                                                       \
                sht_test_pass++;                                               \
        }                                                                      \
        else {                                                                 \
                sht_test_fail++;                                               \
                printf(SHT_TEST_RED "[FAIL (RETURN)]" SHT_TEST_RESET           \
                       " @ " __FILE__ ":%d || (" #condition ")\n", __LINE__);  \
                return;                                                        \
        }                                                                      \
} while (0)

#define SHT_TEST_EXPECT_DEFER(condition) \
do {                                                                           \
        if (condition) {                                                       \
                sht_test_pass++;                                               \
        }                                                                      \
        else {                                                                 \
                sht_test_fail++;                                               \
                printf(SHT_TEST_RED "[FAIL (DEFER)]" SHT_TEST_RESET            \
                       " @ " __FILE__ ":%d || (" #condition ")\n", __LINE__);  \
                goto SHT_TEST_DEFER_LABEL;                                     \
        }                                                                      \
} while (0)

#endif  // SHT_TEST_DEFINED_H

#if defined(SHT_TEST_SINGLE_FILE_ONLY) || defined(SHT_TEST_CONTAINS_MAIN)
size_t sht_test_pass = 0;
size_t sht_test_fail = 0;
size_t sht_test_net_pass = 0;
size_t sht_test_net_fail = 0;
#endif
