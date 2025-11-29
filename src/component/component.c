#include <string.h>
#include <stdlib.h>

#include "component.h"
#include "types.h"

component_t *new_component(enum ctype type) {
    component_t *component = (component_t *) calloc(1, sizeof(component_t));
    if (component == NULL) return NULL;

    component->type = type;
    return component;
}
