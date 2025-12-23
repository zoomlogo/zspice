#pragma once

#include "circuit.h"
#include "component/component.h"
#include "error.h"

error_e dc_solve_linear(circuit_t *circuit);
error_e dc_solve_non_linear(circuit_t *circuit);
