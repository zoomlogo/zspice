/**
 * @file mosfet.c
 */
#include <math.h>
#include <stdio.h>

#include "core/environment.h"
#include "util/error.h"
#include "util/log.h"
#include "util/lu.h"
#include "util/zmth.h"

#include "component.h"
#include "types.h"

error_e mosfet_linearize(component_t *c, env_t *env) {
    log_error("TODO");
    return ERR_UNIMPL;
}

void mosfet_limit(component_t *c) {
    log_error("TODO");
}

#define A(i, j) MI(buf->A, (i), (j), buf->dim)
error_e dc_stamp_mosfet(sbuf_t *buf, component_t *c, env_t *env) {
    log_error("TODO");
    return ERR_UNIMPL;
}
#undef A

#define A(i, j) MI(buf->zA, (i), (j), buf->dim)
error_e ac_stamp_mosfet(sbuf_t *buf, component_t *c, env_t *env) {
    log_error("TODO");
    return ERR_UNIMPL;
}
#undef A
