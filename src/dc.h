#pragma once

#include "circuit.h"
#include "component/component.h"
#include "error.h"

error_t dc_solve_linear(circuit_t *circuit);
error_t dc_solve_non_linear(circuit_t *circuit);
