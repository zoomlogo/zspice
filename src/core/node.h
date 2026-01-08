/**
 * @file node.h
 */
#pragma once
#include "types.h"

/**
 * @brief Stores information about each node.
 */
typedef struct {
    f64 potential; ///< The node's potential.
    f64 phase; ///< The node's phase.
} node_t;
