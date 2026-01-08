/**
 * @file csv.h
 */
#pragma once
#include <stdio.h>

#include "util/error.h"

#include "types.h"

typedef struct {
    FILE *fptr;
    usize cols;
    char **header;
} csv_t;

csv_t *csv_open(const char *filename);
void csv_close(csv_t *csv);

error_e csv_add_header(csv_t *csv, const char *name);
error_e csv_write_header(csv_t *csv);

error_e csv_write_row(csv_t *csv, f64 *row_data);
