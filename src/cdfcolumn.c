#include "cdfcolumn.h"

#include "cdfmem.h"
#include "cdfutils.h"

struct cdfcolumn_t
{
    char *name;          // Name of column
    cdfdt type;          // Data type of column
    cdf_usize nelements; // Number of elements in column
    void *elements;      // Data present in the column. `Total size = nelements * type.size`
    cdf_bool *is_empty;  // `cdf_true` if data missing else `cdf_false`. `Length = nelements`
};

cdf_bool cdfc_create(const char *name, cdfdtk kind, cdfc **out)
{
    cdfc *column;

    if (name == NULL || out == NULL)
        return cdf_false;

    if (!cdf_alloc(sizeof(*column), &column))
        return cdf_false;

    column->name = NULL;
    column->nelements = 0;
    column->elements = NULL;
    column->is_empty = NULL;

    if (!cdf_strdup(name, &column->name))
    {
        cdfc_destroy(&column);
        return cdf_false;
    }

    cdfdt type;
    if (!cdfdt_new(kind, &type))
    {
        cdfc_destroy(&column);
        return cdf_false;
    }
    column->type = type;

    *out = column;
    return cdf_true;
}

void cdfc_destroy(cdfc **c)
{
    if (c == NULL || *c == NULL)
        return;

    cdf_free(&(*c)->name);
    cdf_free(&(*c)->elements);
    cdf_free(&(*c)->is_empty);
    cdf_free(c);
}
