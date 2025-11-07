#pragma once
#include "types.h"

#define V_PRINT(v,N) \
    ({ const f64 *v_ = (v); const usize N_ = (N); \
       for (usize i = 0; i < N_; i++) printf("%lf ", v_[i]); printf("\n"); })
#define M_PRINT(m,N) \
    ({ const f64 **m_ = (m); const usize N_ = (N); \
       for (usize i = 0; i < N; i++) { \
           for (usize j = 0; j < N; j++) { \
               printf("%lf ", m_[i][j]); \
           } \
           printf("\n"); \
       }})

void swap_row(f64 **A, usize N, usize i, usize j);
void lu_decompose(f64 **A, usize N, usize *p);

void low_solve(f64 **L, usize N, f64 *B, usize *p);
void upp_solve(f64 **U, usize N, f64 *Y);

void solve(f64 **A, usize N, f64 *B);
