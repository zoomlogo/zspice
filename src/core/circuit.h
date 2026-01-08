/**
 * @file circuit.h
 */
#pragma once
#include "component/component.h"
#include "util/error.h"

#include "environment.h"
#include "node.h"
#include "types.h"

/**
 * @brief Stores circuits as a collection of nodes and components.
 */
typedef struct {
    usize node_count; ///< The number of nodes present.
    node_t *nodes; ///< Dynamic array of the nodes.

    usize component_capacity; ///< Space allocated for the components.
    usize component_count; ///< Number of components present.
    component_t *components; ///< Dynamic array of components.

    usize dim; ///< The number of unknowns in the circuit.
    env_t default_env; ///< The default simulation environment.
} circuit_t;

/**
 * @brief Creates a new circuit.
 *
 * @returns Pointer to the newly allocated circuit, otherwise `NULL`.
 */
circuit_t *new_circuit(void);
/**
 * @brief Deletes/Frees a circuit.
 *
 * @param circuit The circuit to be deleted.
 */
void del_circuit(circuit_t *circuit);

/**
 * @brief Add a new component to circuit.
 *
 * Dynamically resizes the `nodes` and `components` array if required.
 * The given component is copied into the circuit.
 *
 * @param circuit The circuit to which the component has to be added.
 * @param component The component to be copied into the circuit.
 * @returns OK on success.
 */
error_e c_add_connection(circuit_t *circuit, const component_t *component);
/**
 * @brief Calculate total number of unknowns in the circuit.
 *
 * Calculates the total number of unknowns and hence the dimension of
 * the solver matrices. It also assigns the `solver_id` of the components
 * which require their branch current to be known. The resulting dimension
 * is written to `circuit->dim`.
 *
 * @param circuit The circuit.
 * @returns OK on success.
 */
error_e c_calculate_dim(circuit_t *circuit);
