#include "cdf_test.h"
#include "cdfdf.h"
#include "cdfcolumn.h"
#include "cdftypes.h"
#include "cdferr.h"

#include <string.h>

static cdf_bool make_i32_column(const char *name, cdf_usize nrows, cdfc **out)
{
    cdfc *col = NULL;
    cdferr err = {0};

    if (!cdfc_create(name, CDFDTK_I32, &col, &err))
        return cdf_false;

    for (cdf_usize i = 0; i < nrows; ++i)
    {
        cdf_i32 v = (cdf_i32)i;
        if (!cdfc_append(col, &v, &err))
        {
            cdfc_destroy(&col);
            return cdf_false;
        }
    }

    *out = col;
    return cdf_true;
}

static void test_create_and_destroy(void)
{
    cdfdf *df = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(df != NULL);
    CDF_CHECK(err.code == CDFEC_NONE);

    cdfdf_destroy(&df);
    CDF_CHECK(df == NULL);

    /* destroying an already-NULL dataframe is a safe no-op */
    cdfdf_destroy(&df);
    CDF_CHECK(df == NULL);
}

static void test_create_null_out(void)
{
    cdferr err = {0};

    CDF_CHECK(!cdfdf_create(NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    /* NULL err must be safely accepted */
    CDF_CHECK(!cdfdf_create(NULL, NULL));
}

static void test_add_column_success(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 5, &col));

    CDF_CHECK(cdfdf_add_column(df, &col, &err));
    /* ownership transferred: caller's pointer must be nulled */
    CDF_CHECK(col == NULL);
    CDF_CHECK(err.code == CDFEC_NONE);

    cdfdf_destroy(&df);
}

static void test_add_column_null_args(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 3, &col));

    CDF_CHECK(!cdfdf_add_column(NULL, &col, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_add_column(df, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    /* column must be untouched after failed add */
    CDF_CHECK(col != NULL);

    cdfc_destroy(&col);
    cdfdf_destroy(&df);
}

static void test_add_column_mismatched_rows(void)
{
    cdfdf *df = NULL;
    cdfc *first = NULL;
    cdfc *mismatched = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 5, &first));
    CDF_CHECK(cdfdf_add_column(df, &first, &err));

    CDF_CHECK(make_i32_column("b", 3, &mismatched));
    CDF_CHECK(!cdfdf_add_column(df, &mismatched, &err));
    CDF_CHECK(err.code == CDFEC_ROW_COUNT_MISMATCH);
    /* rejected column remains owned by the caller */
    CDF_CHECK(mismatched != NULL);

    cdfc_destroy(&mismatched);
    cdfdf_destroy(&df);
}

static void test_add_column_duplicate_name(void)
{
    cdfdf *df = NULL;
    cdfc *first = NULL;
    cdfc *dup = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 4, &first));
    CDF_CHECK(cdfdf_add_column(df, &first, &err));

    CDF_CHECK(make_i32_column("a", 4, &dup));
    CDF_CHECK(!cdfdf_add_column(df, &dup, &err));
    CDF_CHECK(err.code == CDFEC_DUPLICATE_NAME);
    CDF_CHECK(dup != NULL);

    cdfc_destroy(&dup);
    cdfdf_destroy(&df);
}

static void test_column_by_index(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    const cdfc *fetched = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 4, &col));
    CDF_CHECK(cdfdf_add_column(df, &col, &err));

    CDF_CHECK(cdfdf_column(df, 0, &fetched, &err));
    CDF_CHECK(fetched != NULL);
    CDF_CHECK(strcmp(cdfc_name(fetched), "a") == 0);
    CDF_CHECK(err.code == CDFEC_NONE);

    /* out of bounds */
    CDF_CHECK(!cdfdf_column(df, 1, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INDEX_OUT_OF_BOUNDS);

    /* NULL args */
    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column(NULL, 0, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column(df, 0, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfdf_destroy(&df);
}

static void test_column_mut_by_index(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    cdfc *fetched = NULL;
    cdf_i32 new_value = 123;
    cdf_i32 out;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 4, &col));
    CDF_CHECK(cdfdf_add_column(df, &col, &err));

    CDF_CHECK(cdfdf_column_mut(df, 0, &fetched, &err));
    CDF_CHECK(fetched != NULL);

    /* mutable handle actually allows mutation */
    CDF_CHECK(cdfc_set(fetched, 0, &new_value, &err));
    CDF_CHECK(cdfc_get(fetched, 0, &out, &err));
    CDF_CHECK(out == new_value);

    CDF_CHECK(!cdfdf_column_mut(df, 99, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INDEX_OUT_OF_BOUNDS);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_mut(NULL, 0, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_mut(df, 0, NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfdf_destroy(&df);
}

static void test_column_by_name(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    const cdfc *fetched = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("age", 4, &col));
    CDF_CHECK(cdfdf_add_column(df, &col, &err));

    CDF_CHECK(cdfdf_column_by_name(df, "age", &fetched, &err));
    CDF_CHECK(fetched != NULL);
    CDF_CHECK(cdfc_nelements(fetched) == 4);
    CDF_CHECK(err.code == CDFEC_NONE);

    /* unknown name fails */
    CDF_CHECK(!cdfdf_column_by_name(df, "nope", &fetched, &err));
    CDF_CHECK(err.code == CDFEC_NOT_FOUND);

    /* NULL args */
    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name(NULL, "age", &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name(df, NULL, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name(df, "age", NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfdf_destroy(&df);
}

static void test_column_by_name_mut(void)
{
    cdfdf *df = NULL;
    cdfc *col = NULL;
    cdfc *fetched = NULL;
    cdf_i32 new_value = 77;
    cdf_i32 out;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("age", 4, &col));
    CDF_CHECK(cdfdf_add_column(df, &col, &err));

    CDF_CHECK(cdfdf_column_by_name_mut(df, "age", &fetched, &err));
    CDF_CHECK(fetched != NULL);

    CDF_CHECK(cdfc_set(fetched, 1, &new_value, &err));
    CDF_CHECK(cdfc_get(fetched, 1, &out, &err));
    CDF_CHECK(out == new_value);

    CDF_CHECK(!cdfdf_column_by_name_mut(df, "nope", &fetched, &err));
    CDF_CHECK(err.code == CDFEC_NOT_FOUND);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name_mut(NULL, "age", &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name_mut(df, NULL, &fetched, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    err = (cdferr){0};
    CDF_CHECK(!cdfdf_column_by_name_mut(df, "age", NULL, &err));
    CDF_CHECK(err.code == CDFEC_INVALID_ARG);

    cdfdf_destroy(&df);
}

static void test_multiple_columns(void)
{
    cdfdf *df = NULL;
    cdfc *a = NULL;
    cdfc *b = NULL;
    const cdfc *fetched = NULL;
    cdferr err = {0};

    CDF_CHECK(cdfdf_create(&df, &err));
    CDF_CHECK(make_i32_column("a", 3, &a));
    CDF_CHECK(make_i32_column("b", 3, &b));

    CDF_CHECK(cdfdf_add_column(df, &a, &err));
    CDF_CHECK(cdfdf_add_column(df, &b, &err));

    CDF_CHECK(cdfdf_column(df, 0, &fetched, &err));
    CDF_CHECK(strcmp(cdfc_name(fetched), "a") == 0);

    CDF_CHECK(cdfdf_column(df, 1, &fetched, &err));
    CDF_CHECK(strcmp(cdfc_name(fetched), "b") == 0);

    cdfdf_destroy(&df);
}

int main(void)
{
    test_create_and_destroy();
    test_create_null_out();
    test_add_column_success();
    test_add_column_null_args();
    test_add_column_mismatched_rows();
    test_add_column_duplicate_name();
    test_column_by_index();
    test_column_mut_by_index();
    test_column_by_name();
    test_column_by_name_mut();
    test_multiple_columns();

    CDF_TEST_REPORT();
}
