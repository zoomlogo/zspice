#include <string.h>
#include <stdlib.h>

#include "component.h"
#include "types.h"

static inline usize p_index(char *p) {
    if (strcmp(p, "resistance") == 0) {
        return 0;
    } else if (strcmp(p, "voltage") == 0) {
        return 1;
    }

    // error:
    return -1;
}

component_t *new_component(enum ctype type) {
    component_t *component = (component_t *) calloc(1, sizeof(component_t));
    component->type = type;
    return component;
}

void p_set(component_t *component, char *p, f64 v) { component->properties[p_index(p)] = v; }
f64 p_get(component_t *component, char *p) { return component->properties[p_index(p)]; }
