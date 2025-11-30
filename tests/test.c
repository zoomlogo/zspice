#include <stdio.h>

#include "types.h"

#include "test_def.h"

usize test_num = 0;
usize passed = 0;

i32 main(void) {
#define X(f) f();
    TESTS(X)
#undef X
    return 0;
}
