#include "error.h"

const char *err_str(error_e error) {
    switch (error) {
#define X(n, m) case n: return m;
        ERRORS(X)
#undef X
        default: return "unknown error code";
    }
}
