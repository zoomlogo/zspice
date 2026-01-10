#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/error.h"

#include "csv.h"
#include "types.h"

csv_t *csv_open(const char *filename) {
    csv_t *csv = (csv_t *) malloc(sizeof(csv_t));
    if (csv == NULL) return NULL;

    csv->fptr = fopen(filename, "w");
    if (csv->fptr == NULL) {
        free(csv);
        return NULL;
    }

    csv->cols = 0;
    csv->cloc = 0;
    csv->header = NULL;

    return csv;
}

void csv_close(csv_t *csv) {
    if (csv == NULL) return;

    fclose(csv->fptr);

    for (usize i = 0; i < csv->cols; i++)
        free(csv->header[i]);
    free(csv->header);
    free(csv);
}

error_e csv_add_header(csv_t *csv, const char *name) {
    if (csv == NULL) return ERR_INVALID_ARG;

    char **new_header = realloc(csv->header, (csv->cols + 1) * sizeof(char *));
    if (new_header == NULL) return ERR_MEM_ALLOC;

    csv->header = new_header;
    csv->header[csv->cols++] = strdup(name);
    return OK;
}

error_e csv_write_header(csv_t *csv) {
    if (csv == NULL) return ERR_INVALID_ARG;

    for (usize i = 0; i < csv->cols; i++) {
        if (i > 0) fprintf(csv->fptr, ",");
        fprintf(csv->fptr, "%s", csv->header[i]);
    }
    fprintf(csv->fptr, "\n");
    return OK;
}

error_e csv_write_data(csv_t *csv, const f64 val) {
    if (csv == NULL) return ERR_INVALID_ARG;

    if (csv->cloc++ > 0) fprintf(csv->fptr, ",");
    fprintf(csv->fptr, "%.6lf", val);

    csv->cloc %= csv->cols;
    if (csv->cloc == 0) fprintf(csv->fptr, "\n");

    return OK;
}

error_e csv_write_row(csv_t *csv, const f64 *row_data) {
    if (csv == NULL) return ERR_INVALID_ARG;
    if (csv->cloc != 0) return ERR_IO;

    for (usize i = 0; i < csv->cols; i++) {
        if (i > 0) fprintf(csv->fptr, ",");
        fprintf(csv->fptr, "%.6lf", row_data[i]);
    }
    fprintf(csv->fptr, "\n");
    return OK;
}
