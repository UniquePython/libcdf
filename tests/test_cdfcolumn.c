#include "cdf_test.h"
#include "cdfcolumn.h"
#include "cdftypes.h"

#include <string.h>

static void test_create_and_destroy(void)
{
    cdfc *col = NULL;

    CDF_CHECK(cdfc_create("age", CDFDTK_I32, &col));
    CDF_CHECK(col != NULL);
    CDF_CHECK(strcmp(cdfc_name(col), "age") == 0);
    CDF_CHECK(cdfc_nelements(col) == 0);

    cdfc_destroy(&col);
    CDF_CHECK(col == NULL);

    /* destroying an already-NULL column is a safe no-op */
    cdfc_destroy(&col);
    CDF_CHECK(col == NULL);
}

static void test_create_null_args(void)
{
    cdfc *col = NULL;

    CDF_CHECK(!cdfc_create(NULL, CDFDTK_I32, &col));
    CDF_CHECK(col == NULL);

    CDF_CHECK(!cdfc_create("age", CDFDTK_I32, NULL));
}

static void test_append_and_get_i32(void)
{
    cdfc *col = NULL;
    cdf_i32 value;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));

    for (cdf_i32 i = 0; i < 10; ++i)
        CDF_CHECK(cdfc_append(col, &i));

    CDF_CHECK(cdfc_nelements(col) == 10);

    for (cdf_i32 i = 0; i < 10; ++i)
    {
        CDF_CHECK(cdfc_get(col, (cdf_usize)i, &value));
        CDF_CHECK(value == i);
    }

    cdfc_destroy(&col);
}

static void test_append_null_args(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 5;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));

    CDF_CHECK(!cdfc_append(NULL, &value));
    CDF_CHECK(!cdfc_append(col, NULL));
    CDF_CHECK(cdfc_nelements(col) == 0);

    cdfc_destroy(&col);
}

static void test_get_out_of_bounds(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 42;
    cdf_i32 out;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));
    CDF_CHECK(cdfc_append(col, &value));

    /* index equal to nelements is out of bounds */
    CDF_CHECK(!cdfc_get(col, 1, &out));
    /* far out of bounds */
    CDF_CHECK(!cdfc_get(col, 999, &out));

    cdfc_destroy(&col);
}

static void test_get_null_args(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 1;
    cdf_i32 out;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));
    CDF_CHECK(cdfc_append(col, &value));

    CDF_CHECK(!cdfc_get(NULL, 0, &out));
    CDF_CHECK(!cdfc_get(col, 0, NULL));

    cdfc_destroy(&col);
}

static void test_set(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 1;
    cdf_i32 new_value = 99;
    cdf_i32 out;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));
    CDF_CHECK(cdfc_append(col, &value));

    CDF_CHECK(cdfc_set(col, 0, &new_value));
    CDF_CHECK(cdfc_get(col, 0, &out));
    CDF_CHECK(out == new_value);

    /* out-of-bounds set fails */
    CDF_CHECK(!cdfc_set(col, 5, &new_value));

    /* NULL args fail */
    CDF_CHECK(!cdfc_set(NULL, 0, &new_value));
    CDF_CHECK(!cdfc_set(col, 0, NULL));

    cdfc_destroy(&col);
}

static void test_reserve(void)
{
    cdfc *col = NULL;

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col));

    CDF_CHECK(cdfc_reserve(col, 100));
    CDF_CHECK(cdfc_nelements(col) == 0); /* reserve doesn't add elements */

    /* still usable after reserving */
    cdf_i32 value = 7;
    CDF_CHECK(cdfc_append(col, &value));
    CDF_CHECK(cdfc_nelements(col) == 1);

    CDF_CHECK(!cdfc_reserve(NULL, 10));

    cdfc_destroy(&col);
}

static void test_different_type_kinds(void)
{
    cdfc *col_dbl = NULL;
    cdfc *col_bool = NULL;
    cdf_dbl d = 3.5;
    cdf_bool b = cdf_true;
    cdf_dbl out_d;
    cdf_bool out_b;

    CDF_CHECK(cdfc_create("dbls", CDFDTK_DBL, &col_dbl));
    CDF_CHECK(cdfc_append(col_dbl, &d));
    CDF_CHECK(cdfc_get(col_dbl, 0, &out_d));
    CDF_CHECK(out_d == d);
    cdfc_destroy(&col_dbl);

    CDF_CHECK(cdfc_create("bools", CDFDTK_BOOL, &col_bool));
    CDF_CHECK(cdfc_append(col_bool, &b));
    CDF_CHECK(cdfc_get(col_bool, 0, &out_b));
    CDF_CHECK(out_b == b);
    cdfc_destroy(&col_bool);
}

int main(void)
{
    test_create_and_destroy();
    test_create_null_args();
    test_append_and_get_i32();
    test_append_null_args();
    test_get_out_of_bounds();
    test_get_null_args();
    test_set();
    test_reserve();
    test_different_type_kinds();

    CDF_TEST_REPORT();
}
