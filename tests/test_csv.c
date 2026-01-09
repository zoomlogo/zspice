#include <stdio.h>

#include "util/error.h"
#include "io/csv.h"

#include "types.h"
#include "test.h"
#include "test_def.h"

static inline void test_csv_simple() {
    // open, write, close
    csv_t *csv = csv_open("test.csv");
    ASSERT(csv != NULL);

    ASSERT_OKC(csv_add_header(csv, "i"));
    ASSERT_OKC(csv_add_header(csv, "f(i)"));
    ASSERT_OKC(csv_write_header(csv));

    f64 dat[] = { 1, 1 };
    ASSERT_OKC(csv_write_row(csv, dat));
    ASSERT_OKC(csv_write_data(csv, 2));

    error_e err = csv_write_row(csv, dat);
    ASSERT(err == ERR_IO);

    ASSERT_OKC(csv_write_data(csv, 4));
    csv_close(csv);

    // dump, delete
    FILE *f = fopen("test.csv", "r");
    int c;
    while ((c = fgetc(f)) != EOF) putchar(c);
    fclose(f);
    remove("test.csv");

    return;
err:
    csv_close(csv);
}

void test_csv() {
    BEGIN_TEST();

    test_csv_simple();

    END_TEST();
}
