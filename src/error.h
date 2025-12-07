#pragma once

#define ERRORS(X) \
    X(OK, "ok: operation successful") \
    X(ERR_INVALID_ARG, "arg: invalid arguments supplied") \
    X(ERR_INVALID_PARAM, "param: invalid parameters supplied") \
    X(ERR_MEM_ALLOC, "mem_alloc: failed to allocate sufficient memory") \
    X(ERR_SINGULAR, "singular: matrix was singular") \
    X(ERR_NOT_INIT, "not_init: circuit solver matrix not initialized") \
    X(ERR_DIV_0, "div_0: division by zero") \
    X(ERR_UNIMPL, "unimpl: not implemented yet")

typedef enum {
#define X(n, m) n,
    ERRORS(X)
#undef X
} error_t;

const char *err_str(error_t error);
