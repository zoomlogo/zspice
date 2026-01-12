/**
 * @file component.h
 * @brief Component definitions.
 */
#pragma once
#include "core/environment.h"
#include "core/sbuf.h"
#include "util/error.h"

#include "types.h"

/**
 * @brief Enumeration of all the different circuit component types.
 *
 * The component names are automatically generated from `component.def` using the C preprocessor.
 * It serves as the unique identifier for each component.
 *
 * @note To add a new component, edit `component.def`.
 */
enum ctype {
#define COMPONENT(en, sn, av, p) en,
#include "component.def"
#undef COMPONENT
    _C_LEN //!< The total number of component types.
};

/**
 * @name Component structures generator.
 * @brief Auto-generates the data structures for each circuit component defined in `component.def`.
 * @{
 */

/**
 * @brief Property Macro: Defines a single member in each struct.
 *
 * @param t Type of the variable/parameter.
 * @param x Name of the variable/parameter.
 * @param d Default value of the variable/parameter. (unused here)
 */
#define P(t, x, d) t x;
/**
 * @brief Component Macro: Generates the full struct definition.
 *
 * @param en Enum ID. (unused here)
 * @param sn Struct name.
 * @param av "Access value", name used to access the component in the `component_t` struct. (unused here)
 * @param p Parameter list using P() macros.
 */
#define COMPONENT(en, sn, av, p) struct sn { p };
#include "component.def"
#undef COMPONENT
#undef P
/** @} */

/**
 * @brief Represents a generic circuit component.
 *
 * This is the main data structure for the netlist.
 * It holds the IDs of the nodes it is connected to
 * and the physical parameters as an anonymous union.
 */
typedef struct {
    enum ctype type; //!< The type of component stored.

    /**
     * @brief ID of the (generally) positive node.
     *
     * For other components:
     * - **Diodes:** Anode.
     * - **BJT:** Base.
     *
     * @note Node 0 is assumed to be ground (the reference). So the real MNA Index is `id0 - 1`.
     *       Current is defined as leaving this node for sources.
     */
    usize id0;
    /**
     * @brief ID of the (generally) negative node.
     *
     * For other components:
     * - **Diodes:** Cathode.
     * - **BJT:** Emitter.
     *
     * @note Node 0 is assumed to be ground (the reference). So the real MNA Index is `id1 - 1`.
     */
    usize id1;

    /**
     * @brief Auxiliary Node 1.
     *
     * Meaning depends on component type:
     * - **VCCS/VCVS:** Positive controlling node.
     * - **CCCS/CCVS:** Solver ID of the controlling wire.
     * - **BJT:** Collector.
     */
    usize id2;
    /**
     * @brief Auxiliary Node 2.
     *
     * Meaning depends on component type:
     * - **VCCS/VCVS:** Negative controlling node.
     */
    usize id3;

    /**
     * @brief The MNA row/column index for this component's branch current.
     *
     * This is used for components which require their branch current to be known.
     * They include:
     * - Voltage sources.
     * - Inductors.
     * @note If `solver_id == 0` then the component does not need to know its branch current.
     */
    usize solver_id;

    /**
     * @brief Type-specific physical parameters.
     *
     * Anonymous union generated from `component.def`.
     * Members are accessed with the help of the short name defined in the `component.def` file.
     */
    union {
#define COMPONENT(en, sn, av, p) struct sn av;
#include "component.def"
#undef COMPONENT
    };
} component_t;

component_t *new_component(enum ctype type);

// dc analysis: stamp functions
typedef error_e (*dc_stamp_f)(sbuf_t *, component_t *, env_t *);
#define COMPONENT(en, sn, av, p) error_e dc_stamp_##sn(sbuf_t *buf, component_t *c, env_t *env);
#include "component.def"
#undef COMPONENT

// dc analysis: LUT
extern const dc_stamp_f DC_STAMPS[_C_LEN]; ///< The DC Stamp Lookup Table.

// ac analysis: stamp functions
typedef error_e (*ac_stamp_f)(sbuf_t *, component_t *, env_t *);
#define COMPONENT(en, sn, av, p) error_e ac_stamp_##sn(sbuf_t *buf, component_t *c, env_t *env);
#include "component.def"
#undef COMPONENT

// ac analysis: LUT
extern const ac_stamp_f AC_STAMPS[_C_LEN]; ///< The AC Stamp Lookup Table.

/**
 * @brief Set defaults of a component.
 *
 * Set the default properties of the component as defined by
 * the third argument of the P() macro.
 *
 * @param component Pointer to the component to set the defaults for.
 */
void c_defaults(component_t *component);

// non-linear extra functions
error_e diode_linearize(component_t *c, env_t *env);
void diode_limit(component_t *c, f64 Vj, f64 *r_Vj);

error_e bjt_linearize(component_t *c, env_t *env);
void bjt_limit(component_t *c, f64 Vbe, f64 Vbc, f64 *r_Vbe, f64 *r_Vce);
