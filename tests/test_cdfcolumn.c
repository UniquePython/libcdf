#include "cdf_test.h"
#include "cdfcolumn.h"
#include "cdftypes.h"
#include "cdferr.h"

#include <string.h>

static void test_create_and_destroy(void)
{
    cdfc *col = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("age", CDFDTK_I32, &col, &err));
    CDF_CHECK(col != NULL);
    CDF_CHECK(strcmp(cdfc_name(col), "age") == 0);
    CDF_CHECK(cdfc_nelements(col) == 0);
    CDF_CHECK(err.code == CDFEC_NONE);

    cdfc_destroy(&col);
    CDF_CHECK(col == NULL);

    /* destroying an already-NULL column is a safe no-op */
    cdfc_destroy(&col);
    CDF_CHECK(col == NULL);
}

static void test_create_null_args(void)
{
    cdfc *col = NULL;
    cdferr err = {0};

    CDF_CHECK(!cdfc_create(NULL, CDFDTK_I32, &col, &err));
    CDF_CHECK(col == NULL);
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfc_create("age", CDFDTK_I32, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    /* NULL err must be safely accepted */
    CDF_CHECK(!cdfc_create(NULL, CDFDTK_I32, &col, NULL));
}

static void test_append_and_get_i32(void)
{
    cdfc *col = NULL;
    cdf_i32 value;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));

    for (cdf_i32 i = 0; i < 10; ++i)
        CDF_CHECK(cdfc_append(col, &i, &err));

    CDF_CHECK(cdfc_nelements(col) == 10);
    CDF_CHECK(err.code == CDFEC_NONE);

    for (cdf_i32 i = 0; i < 10; ++i)
    {
        CDF_CHECK(cdfc_get(col, (cdf_usize)i, &value, &err));
        CDF_CHECK(value == i);
    }
    CDF_CHECK(err.code == CDFEC_NONE);

    cdfc_destroy(&col);
}

static void test_append_null_args(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 5;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));

    CDF_CHECK(!cdfc_append(NULL, &value, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfc_append(col, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    CDF_CHECK(cdfc_nelements(col) == 0);

    cdfc_destroy(&col);
}

static void test_get_out_of_bounds(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 42;
    cdf_i32 out;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));
    CDF_CHECK(cdfc_append(col, &value, &err));

    /* index equal to nelements is out of bounds */
    CDF_CHECK(!cdfc_get(col, 1, &out, &err));
    CDF_CHECK(err.code == CDFEC_INDEX_OUT_OF_BOUNDS);

    /* far out of bounds */
    err = (cdferr){0};
    CDF_CHECK(!cdfc_get(col, 999, &out, &err));
    CDF_CHECK(err.code == CDFEC_INDEX_OUT_OF_BOUNDS);

    cdfc_destroy(&col);
}

static void test_get_null_args(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 1;
    cdf_i32 out;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));
    CDF_CHECK(cdfc_append(col, &value, &err));

    CDF_CHECK(!cdfc_get(NULL, 0, &out, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfc_get(col, 0, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfc_destroy(&col);
}

static void test_set(void)
{
    cdfc *col = NULL;
    cdf_i32 value = 1;
    cdf_i32 new_value = 99;
    cdf_i32 out;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));
    CDF_CHECK(cdfc_append(col, &value, &err));

    CDF_CHECK(cdfc_set(col, 0, &new_value, &err));
    CDF_CHECK(cdfc_get(col, 0, &out, &err));
    CDF_CHECK(out == new_value);
    CDF_CHECK(err.code == CDFEC_NONE);

    /* out-of-bounds set fails */
    CDF_CHECK(!cdfc_set(col, 5, &new_value, &err));
    CDF_CHECK(err.code == CDFEC_INDEX_OUT_OF_BOUNDS);

    /* NULL args fail */
    err = (cdferr){0};
    CDF_CHECK(!cdfc_set(NULL, 0, &new_value, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfc_set(col, 0, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfc_destroy(&col);
}

static void test_reserve(void)
{
    cdfc *col = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfc_create("values", CDFDTK_I32, &col, &err));

    CDF_CHECK(cdfc_reserve(col, 100, &err));
    CDF_CHECK(cdfc_nelements(col) == 0); /* reserve doesn't add elements */
    CDF_CHECK(err.code == CDFEC_NONE);

    /* still usable after reserving */
    cdf_i32 value = 7;
    CDF_CHECK(cdfc_append(col, &value, &err));
    CDF_CHECK(cdfc_nelements(col) == 1);

    CDF_CHECK(!cdfc_reserve(NULL, 10, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

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
    cdferr err = {0};

    CDF_CHECK(cdfc_create("dbls", CDFDTK_DBL, &col_dbl, &err));
    CDF_CHECK(cdfc_append(col_dbl, &d, &err));
    CDF_CHECK(cdfc_get(col_dbl, 0, &out_d, &err));
    CDF_CHECK(out_d == d);
    cdfc_destroy(&col_dbl);

    CDF_CHECK(cdfc_create("bools", CDFDTK_BOOL, &col_bool, &err));
    CDF_CHECK(cdfc_append(col_bool, &b, &err));
    CDF_CHECK(cdfc_get(col_bool, 0, &out_b, &err));
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
