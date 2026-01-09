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
    c64 zpotential; ///< The node's complex potential (for AC analysis).
} node_t;
