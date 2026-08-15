#include "cdfdf.h"

#include "cdfmem.h"

#include "cdftypes.h"
#include "cdfcolumn.h"

struct cdfdf_t
{
    cdfc **cols;     // All the columns present in the data frame
    cdf_usize ncols; // Number of columns present in the dataframe
    cdf_usize nrows; // Cached number of rows; every column has exactly nrows elements
};

cdf_bool cdfdf_create(cdfdf **out)
{
    cdfdf *df;

    if (out == NULL)
        return cdf_false;

    if (!cdf_alloc(sizeof(*df), &df))
        return cdf_false;

    df->cols = NULL;
    df->ncols = 0;
    df->nrows = 0;

    *out = df;

    return cdf_true;
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

cdf_bool cdfdf_add_column(cdfdf *df, cdfc **column)
{
    cdf_usize new_ncols;
    cdfc **new_cols;

    if (df == NULL || column == NULL || *column == NULL)
        return cdf_false;

    if (df->ncols > 0 && cdfc_nelements(*column) != df->nrows)
        return cdf_false;

    for (cdf_usize i = 0; i < df->ncols; ++i)
        if (strcmp(cdfc_name(df->cols[i]), cdfc_name(*column)) == 0)
            return cdf_false;

    if (df->ncols == SIZE_MAX)
        return cdf_false;

    new_ncols = df->ncols + 1;

    if (new_ncols > SIZE_MAX / sizeof(*df->cols))
        return cdf_false;

    new_cols = df->cols;

    if (!cdf_realloc(new_ncols * sizeof(*df->cols), &new_cols))
        return cdf_false;

    df->cols = new_cols;
    df->cols[df->ncols] = *column;
    *column = NULL;

    df->ncols = new_ncols;

    if (df->ncols == 1)
        df->nrows = cdfc_nelements(df->cols[0]);

    return cdf_true;
}
