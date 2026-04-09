#include <stdio.h>

#include "util/error.h"
#include "io/csv.h"

#include "types.h"
#include "test_def.h"
#include "sht_test.h"

TEST_DEFINE(csv_simple)
{
	// open, write, close
	csv_t *csv = csv_open("test.csv");
	TEST_EXPECT_RETURN(csv != NULL);

	TEST_EXPECT_DEFER(csv_add_header(csv, "i") == OK);
	TEST_EXPECT_DEFER(csv_add_header(csv, "f(i)") == OK);
	TEST_EXPECT_DEFER(csv_write_header(csv) == OK);

	f64 dat[] = { 1, 1 };
	TEST_EXPECT_DEFER(csv_write_row(csv, dat) == OK);
	TEST_EXPECT_DEFER(csv_write_data(csv, 2) == OK);

	error_e err = csv_write_row(csv, dat);
	TEST_EXPECT(err == ERR_IO);

	TEST_EXPECT_DEFER(csv_write_data(csv, 4) == OK);
	csv_close(csv);

	// dump, delete
	FILE *f = fopen("test.csv", "r");
	int c;
	while ((c = fgetc(f)) != EOF)
		putchar(c);
	fclose(f);
	remove("test.csv");

	return;
defer:
	csv_close(csv);
}

void test_csv()
{
	TEST_BEGIN();

	TEST_RUN(csv_simple);

	TEST_END();
}
