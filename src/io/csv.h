/**
 * @file csv.h
 */
#pragma once
#include <stdio.h>

#include "util/error.h"

#include "types.h"

/**
 * @brief CSV File type.
 */
typedef struct {
    FILE *fptr; ///< `FILE` pointer to the CSV file.
    usize cols; ///< Number of columns in the CSV file.
    char **header; ///< The header of the CSV file.
    usize cloc; ///< Used to determine which column data we are writing.
} csv_t;

/**
 * @brief Open a CSV file for writing.
 *
 * @param filename The name of the file to (over)write.
 * @returns A pointer to `csv_t` if everything works, else NULL.
 */
csv_t *csv_open(const char *filename);
/**
 * @brief Close and free memory of `csv_t`.
 *
 * @param csv CSV type.
 */
void csv_close(csv_t *csv);

/**
 * @brief Add a new column.
 *
 * Adds a column to the `csv_t` which is written
 * later with csv_write_header().
 *
 * @param csv CSV type.
 * @param name Column title.
 * @returns OK on success.
 */
error_e csv_add_header(csv_t *csv, const char *name);
/**
 * @brief Writes the CSV header.
 *
 * @param csv CSV type, with all header added.
 * @returns OK on success.
 */
error_e csv_write_header(csv_t *csv);

/**
 * @brief Write a single data entry.
 *
 * The row end is automatically detected.
 *
 * @param csv CSV type.
 * @param val Value to write.
 * @returns OK on success.
 */
error_e csv_write_data(csv_t *csv, const f64 val);
/**
 * @brief Writes a row of data to the CSV file.
 *
 * @note `row_data` must be equal to (or more than) the number of columns.
 *
 * @param csv CSV type.
 * @param row_data The data of the entire row.
 * @returns OK on success.
 */
error_e csv_write_row(csv_t *csv, const f64 *row_data);
