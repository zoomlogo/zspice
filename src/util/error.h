/**
 * @file error.h
 */
#pragma once

/**
 * @brief List of errors.
 *
 * Defines a list of errors using macro X() which takes in the
 * enum name as the first parameter and the error message as the
 * second parameter.
 */
#define ERRORS(X) \
    X(OK, "ok: operation successful") \
    X(ERR_INVALID_ARG, "arg: invalid arguments supplied") \
    X(ERR_INVALID_PARAM, "param: invalid parameters supplied") \
    X(ERR_MEM_ALLOC, "mem_alloc: failed to allocate sufficient memory") \
    X(ERR_SINGULAR, "singular: matrix was singular") \
    X(ERR_NOT_INIT, "not_init: circuit solver matrix not initialized") \
    X(ERR_DIV_0, "div_0: division by zero") \
    X(ERR_UNIMPL, "unimpl: not implemented yet") \
    X(ERR_IO, "io: io error")

/**
 * @brief The error enum.
 *
 * All possible error codes are defined in the enum using ERRORS().
 */
typedef enum {
#define X(n, m) n,
    ERRORS(X)
#undef X
} error_e;

/**
 * @brief Gives an error message corresponding to an error code.
 *
 * @param error The error code.
 * @return The error message.
 */
const char *err_str(error_e error);
