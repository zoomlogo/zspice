#include <stdio.h>

#include "types.h"

#include "test_def.h"

usize test_num = 0;
usize passed = 0;
usize net_failed = 0;

i32 main(void) {
#define X(f) f();
    TESTS(X)
#undef X
    if (net_failed > 0)
        printf("\033[31mtotal failed: %lu\033[0m\n", net_failed);
    else
        printf("\033[32m======= ALL PASSED! =======\033[0m\n");
    return net_failed;
}
