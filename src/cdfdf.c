#include "cdfdf.h"

#include "cdfmem.h"

#include <string.h>

struct cdfdf_t
{
    cdfc **cols;     // All the columns present in the data frame
    cdf_usize ncols; // Number of columns present in the dataframe
    cdf_usize nrows; // Cached number of rows; every column has exactly nrows elements
};

cdf_bool cdfdf_create(cdfdf **out, cdferr *err)
{
    cdfdf *df;

    cdf_handle_null(out);
    cdf_handle_fail(!cdf_alloc_one(&df), CDFEC_ALLOC_FAILED, "failed to allocate memory for data frame");

    df->cols = NULL;
    df->ncols = 0;
    df->nrows = 0;

    *out = df;

    cdf_success;
}

void cdfdf_destroy(cdfdf **df)
{
    if (df == NULL || *df == NULL)
        return;

    for (cdf_usize i = 0; i < (*df)->ncols; ++i)
        cdfc_destroy(&(*df)->cols[i]);

    cdf_free(&(*df)->cols);
    cdf_free(df);
}

cdf_bool cdfdf_add_column(cdfdf *df, cdfc **column, cdferr *err)
{
    cdf_usize new_ncols;
    cdfc **new_cols;

    cdf_handle_null_3(df, column, *column);

    cdf_handle_fail(df->ncols > 0 && cdfc_nelements(*column) != df->nrows,
                    CDFEC_ROW_COUNT_MISMATCH, "column element count does not match data frame row count");

    for (cdf_usize i = 0; i < df->ncols; ++i)
        cdf_handle_fail(strcmp(cdfc_name(df->cols[i]), cdfc_name(*column)) == 0,
                        CDFEC_DUPLICATE_NAME, "a column with this name already exists");

    cdf_handle_fail(df->ncols == SIZE_MAX, CDFEC_SIZE_OVERFLOW, "column count overflows");

    new_ncols = df->ncols + 1;

    cdf_handle_fail(new_ncols > SIZE_MAX / sizeof(*df->cols), CDFEC_SIZE_OVERFLOW, "column array size overflows");

    new_cols = df->cols;

    cdf_handle_fail(!cdf_realloc(new_ncols * sizeof(*df->cols), &new_cols),
                    CDFEC_ALLOC_FAILED, "failed to reallocate column array");

    df->cols = new_cols;
    df->cols[df->ncols] = *column;
    *column = NULL;

    df->ncols = new_ncols;

    if (df->ncols == 1)
        df->nrows = cdfc_nelements(df->cols[0]);

    cdf_success;
}

cdf_bool cdfdf_column(const cdfdf *df, cdf_usize index, const cdfc **out, cdferr *err)
{
    cdf_handle_null_2(df, out);
    cdf_handle_fail(index >= df->ncols, CDFEC_INDEX_OUT_OF_BOUNDS, "index is out of bounds");

    *out = df->cols[index];

    cdf_success;
}

cdf_bool cdfdf_column_mut(cdfdf *df, cdf_usize index, cdfc **out, cdferr *err)
{
    cdf_handle_null_2(df, out);
    cdf_handle_fail(index >= df->ncols, CDFEC_INDEX_OUT_OF_BOUNDS, "index is out of bounds");

    *out = df->cols[index];

    cdf_success;
}

cdf_bool cdfdf_column_by_name(const cdfdf *df, const char *name, const cdfc **out, cdferr *err)
{
    cdf_handle_null_3(df, name, out);

    for (cdf_usize i = 0; i < df->ncols; ++i)
    {
        if (strcmp(cdfc_name(df->cols[i]), name) == 0)
        {
            *out = df->cols[i];
            cdf_success;
        }
    }

    cdf_fail(CDFEC_NOT_FOUND, "no column with this name exists");
}

cdf_bool cdfdf_column_by_name_mut(cdfdf *df, const char *name, cdfc **out, cdferr *err)
{
    cdf_handle_null_3(df, name, out);

    for (cdf_usize i = 0; i < df->ncols; ++i)
    {
        if (strcmp(cdfc_name(df->cols[i]), name) == 0)
        {
            *out = df->cols[i];
            cdf_success;
        }
    }

    cdf_fail(CDFEC_NOT_FOUND, "no column with this name exists");
}