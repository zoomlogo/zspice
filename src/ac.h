#pragma once

#include "circuit.h"
#include "component/component.h"
#include "error.h"

error_e ac_solve(circuit_t *circuit, f64 w);
error_e ac_sweep(circuit_t *circuit, f64 start_freq, f64 stop_freq, f64 step);
