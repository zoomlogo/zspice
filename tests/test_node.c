#include <stdio.h>
#include <stdlib.h>

#include "node.h"

#include "test.h"
#include "test_def.h"

void test_node(void) {
    BEGIN_TEST();

    node_t *node = new_node();
    ASSERT(node != NULL);

    ASSERT(node->is_grounded == false);
    ASSERT(node->potential == 0);
    ASSERT(node->solver_id == 0);
    ASSERT(node->id == 0);

    del_node(node);

    END_TEST();
}
