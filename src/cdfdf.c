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
