#include "test.h"
#include "test_def.h"

#include "circuit.h"
#include "types.h"

void test_circuit(void) {
    BEGIN_TEST();

    circuit_t *circuit = new_circuit(4);

    ASSERT(circuit != NULL);
    ASSERT(circuit->N == 4);
    ASSERT(circuit->_n == 0);

    del_circuit(circuit);

    END_TEST();
}
